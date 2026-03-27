#!/usr/bin/env python3
"""
Flight Simulator - асинхронный симулятор полёта БПЛА.

Задание: реализуйте логику движения между waypoints
и отправку MAVLink телеметрии.
"""

import argparse
import asyncio
import json
import math
import struct
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


@dataclass
class Waypoint:
    """Точка маршрута."""
    lat: float
    lon: float
    alt: float
    speed: float = 10.0


@dataclass
class DroneState:
    """Состояние дрона."""
    lat: float = 55.7558
    lon: float = 37.6173
    alt: float = 0.0
    
    vx: float = 0.0
    vy: float = 0.0
    vz: float = 0.0
    
    roll: float = 0.0
    pitch: float = 0.0
    yaw: float = 0.0
    heading: int = 0
    
    groundspeed: float = 0.0
    airspeed: float = 0.0
    
    battery: float = 100.0
    battery_voltage: float = 12.6
    
    armed: bool = False
    mode: str = "STABILIZE"
    
    home_lat: float = 55.7558
    home_lon: float = 37.6173


class MAVLinkSender:
    """Отправщик MAVLink сообщений."""
    
    MAGIC = 0xFE
    
    def __init__(self, system_id: int = 1, component_id: int = 1):
        self.system_id = system_id
        self.component_id = component_id
        self.sequence = 0
    
    @staticmethod
    def crc_accumulate(byte: int, crc: int) -> int:
        tmp = byte ^ (crc & 0xFF)
        tmp ^= (tmp << 4) & 0xFF
        return ((crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4)) & 0xFFFF
    
    def _pack(self, msg_id: int, payload: bytes, crc_extra: int) -> bytes:
        """Упаковать MAVLink v1 пакет."""
        header = struct.pack('<BBBBBB',
            self.MAGIC, len(payload), self.sequence,
            self.system_id, self.component_id, msg_id
        )
        self.sequence = (self.sequence + 1) & 0xFF
        
        crc = 0xFFFF
        for b in header[1:] + payload:
            crc = self.crc_accumulate(b, crc)
        crc = self.crc_accumulate(crc_extra, crc)
        
        return header + payload + struct.pack('<H', crc)
    
    def heartbeat(self, state: DroneState) -> bytes:
        """HEARTBEAT сообщение."""
        mode_map = {"STABILIZE": 80, "GUIDED": 88, "AUTO": 92, "LAND": 93, "RTL": 94}
        custom_mode = mode_map.get(state.mode, 80)
        base_mode = 0x80 | (0x10 if state.armed else 0)
        
        payload = struct.pack('<IBBBBB',
            custom_mode, 2, 3, base_mode, 4 if state.armed else 3, 3
        )
        return self._pack(0, payload, 50)
    
    def global_position_int(self, state: DroneState) -> bytes:
        """GLOBAL_POSITION_INT сообщение."""
        time_ms = int(time.time() * 1000) & 0xFFFFFFFF
        payload = struct.pack('<IiiiihhhH',
            time_ms,
            int(state.lat * 1e7), int(state.lon * 1e7),
            int(state.alt * 1000), int(state.alt * 1000),
            int(state.vx * 100), int(state.vy * 100), int(state.vz * 100),
            state.heading * 100
        )
        return self._pack(33, payload, 104)
    
    def vfr_hud(self, state: DroneState) -> bytes:
        """VFR_HUD сообщение."""
        payload = struct.pack('<ffffhH',
            state.airspeed, state.groundspeed, state.alt, state.vz, state.heading, 50
        )
        return self._pack(74, payload, 20)


class FlightSimulator:
    """Асинхронный симулятор полёта."""
    
    # Физические параметры
    MAX_SPEED = 15.0  # м/с
    ACCELERATION = 2.0  # м/с²
    CLIMB_RATE = 3.0  # м/с
    WP_RADIUS = 5.0  # м
    
    def __init__(self, system_id: int = 1):
        self.state = DroneState()
        self.mission: list[Waypoint] = []
        self.current_wp: int = 0
        self.mission_active: bool = False
        self.paused: bool = False
        self.running: bool = False
        
        self.mavlink = MAVLinkSender(system_id=system_id)
        self.transport: Optional[asyncio.DatagramTransport] = None
        self.telemetry_addr: tuple = ('127.0.0.1', 14551)
    
    async def load_mission(self, filepath: str) -> bool:
        """
        Загрузить миссию из JSON файла.
        
        TODO: Реализуйте загрузку миссии
        
        Формат файла:
        {
            "home": {"lat": ..., "lon": ..., "alt": ...},
            "waypoints": [{"lat": ..., "lon": ..., "alt": ..., "speed": ...}, ...]
        }
        
        После загрузки:
        - Установить home позицию в state
        - Заполнить self.mission списком Waypoint
        """
        # Ваш код здесь
        
        return False
    
    def arm(self) -> bool:
        """
        Армировать дрон.
        
        TODO: Реализуйте армирование
        
        Условия:
        - Нельзя армировать если уже армирован
        - После армирования: armed=True, mode="GUIDED"
        """
        # Ваш код здесь
        
        return False
    
    def disarm(self) -> bool:
        """
        Дизармировать дрон.
        
        TODO: Реализуйте дизармирование
        
        Условия:
        - Нельзя дизармировать в воздухе (alt > 1м)
        - После дизармирования: armed=False, mode="STABILIZE"
        """
        # Ваш код здесь
        
        return False
    
    def start_mission(self) -> bool:
        """
        Начать выполнение миссии.
        
        TODO: Реализуйте запуск миссии
        
        Условия:
        - Должен быть армирован
        - Должна быть загружена миссия
        - После запуска: mode="AUTO", mission_active=True
        """
        # Ваш код здесь
        
        return False
    
    def pause_mission(self):
        """Приостановить миссию."""
        if self.mission_active:
            self.paused = True
            self.state.mode = "GUIDED"
    
    def resume_mission(self):
        """Возобновить миссию."""
        if self.paused:
            self.paused = False
            self.state.mode = "AUTO"
    
    @staticmethod
    def haversine_distance(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
        """Расстояние между точками в метрах."""
        R = 6371000
        phi1, phi2 = math.radians(lat1), math.radians(lat2)
        dphi = math.radians(lat2 - lat1)
        dlam = math.radians(lon2 - lon1)
        
        a = math.sin(dphi/2)**2 + math.cos(phi1) * math.cos(phi2) * math.sin(dlam/2)**2
        return R * 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
    
    @staticmethod
    def bearing(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
        """Азимут от точки 1 к точке 2 в радианах."""
        phi1, phi2 = math.radians(lat1), math.radians(lat2)
        dlam = math.radians(lon2 - lon1)
        
        y = math.sin(dlam) * math.cos(phi2)
        x = math.cos(phi1) * math.sin(phi2) - math.sin(phi1) * math.cos(phi2) * math.cos(dlam)
        return math.atan2(y, x)
    
    def update_physics(self, dt: float):
        """
        Обновить физику движения.
        
        TODO: Реализуйте физику движения
        
        Шаги:
        1. Если не армирован или миссия не активна - выход
        2. Получить текущий waypoint
        3. Вычислить расстояние и направление
        4. Если расстояние < WP_RADIUS - перейти к следующему
        5. Вычислить желаемую скорость (с учётом ускорения)
        6. Обновить позицию: lat += dlat, lon += dlon
        7. Обновить высоту (с учётом CLIMB_RATE)
        8. Обновить heading, groundspeed
        9. Разряд батареи: battery -= 0.1 * dt
        """
        if not self.state.armed:
            return
        
        if not self.mission_active or self.paused:
            return
        
        # Ваш код здесь
        pass
    
    async def telemetry_loop(self):
        """Цикл отправки телеметрии."""
        while self.running:
            if self.transport:
                messages = [
                    self.mavlink.heartbeat(self.state),
                    self.mavlink.global_position_int(self.state),
                    self.mavlink.vfr_hud(self.state),
                ]
                for msg in messages:
                    self.transport.sendto(msg, self.telemetry_addr)
            
            await asyncio.sleep(0.1)  # 10 Hz
    
    async def physics_loop(self):
        """Цикл обновления физики."""
        last_time = time.time()
        
        while self.running:
            current_time = time.time()
            dt = current_time - last_time
            last_time = current_time
            
            self.update_physics(dt)
            
            await asyncio.sleep(0.05)  # 20 Hz
    
    async def command_handler(self, data: bytes, addr: tuple):
        """Обработчик входящих команд."""
        try:
            cmd = json.loads(data.decode())
            command = cmd.get('command', '')
            
            if command == 'arm':
                success = self.arm()
                print(f"ARM: {'OK' if success else 'FAILED'}")
            elif command == 'disarm':
                success = self.disarm()
                print(f"DISARM: {'OK' if success else 'FAILED'}")
            elif command == 'start':
                success = self.start_mission()
                print(f"START: {'OK' if success else 'FAILED'}")
            elif command == 'pause':
                self.pause_mission()
                print("PAUSED")
            elif command == 'resume':
                self.resume_mission()
                print("RESUMED")
            else:
                print(f"Unknown command: {command}")
        except Exception as e:
            print(f"Command error: {e}")
    
    async def run(self, telemetry_port: int = 14550, command_port: int = 14551):
        """Запустить симулятор."""
        self.running = True
        self.telemetry_addr = ('127.0.0.1', telemetry_port)
        
        # Создаём UDP сокет для телеметрии
        loop = asyncio.get_event_loop()
        
        class TelemetryProtocol(asyncio.DatagramProtocol):
            def __init__(self, simulator):
                self.simulator = simulator
            
            def connection_made(self, transport):
                self.simulator.transport = transport
        
        transport, _ = await loop.create_datagram_endpoint(
            lambda: TelemetryProtocol(self),
            local_addr=('0.0.0.0', 0)
        )
        
        # Создаём UDP сокет для команд
        class CommandProtocol(asyncio.DatagramProtocol):
            def __init__(self, simulator):
                self.simulator = simulator
            
            def datagram_received(self, data, addr):
                asyncio.create_task(self.simulator.command_handler(data, addr))
        
        await loop.create_datagram_endpoint(
            lambda: CommandProtocol(self),
            local_addr=('0.0.0.0', command_port)
        )
        
        print(f"Flight Simulator started")
        print(f"Telemetry -> {self.telemetry_addr}")
        print(f"Commands <- 0.0.0.0:{command_port}")
        
        # Запускаем основные циклы
        try:
            await asyncio.gather(
                self.telemetry_loop(),
                self.physics_loop(),
            )
        except asyncio.CancelledError:
            pass
        finally:
            self.running = False
            transport.close()


async def main():
    parser = argparse.ArgumentParser(description='Flight Simulator')
    parser.add_argument('--mission', type=str, help='Mission JSON file')
    parser.add_argument('--telemetry-port', type=int, default=14550)
    parser.add_argument('--command-port', type=int, default=14551)
    parser.add_argument('--system-id', type=int, default=1)
    
    args = parser.parse_args()
    
    simulator = FlightSimulator(system_id=args.system_id)
    
    if args.mission:
        success = await simulator.load_mission(args.mission)
        if success:
            print(f"Mission loaded: {len(simulator.mission)} waypoints")
        else:
            print("Failed to load mission")
    
    try:
        await simulator.run(args.telemetry_port, args.command_port)
    except KeyboardInterrupt:
        print("\nShutting down...")


if __name__ == '__main__':
    asyncio.run(main())
