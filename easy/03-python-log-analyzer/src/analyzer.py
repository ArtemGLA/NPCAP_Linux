"""
Flight Log Analyzer - анализатор полётных логов для обнаружения аномалий.
"""

import pandas as pd
import numpy as np
from typing import Any, Dict, List, Optional


def load_flight_log(filepath: str) -> pd.DataFrame:
    """
    Загружает CSV файл с данными полёта.
    
    Args:
        filepath: путь к CSV файлу
        
    Returns:
        DataFrame с колонками: timestamp, lat, lon, alt, relative_alt,
        groundspeed, airspeed, heading, roll, pitch, yaw,
        battery_voltage, battery_remaining, gps_fix, satellites
    """

    df = pd.read_csv(filepath)
    return df



def detect_altitude_anomalies(df: pd.DataFrame, threshold: float = 10.0) -> List[Dict]:
    """
    Находит резкие изменения высоты.
    
    Аномалия: изменение relative_alt более чем на threshold метров
    за 1 секунду (между соседними записями).
    
    Args:
        df: DataFrame с данными полёта
        threshold: порог изменения высоты в метрах
        
    Returns:
        Список словарей с полями:
        - timestamp: время аномалии
        - old_value: предыдущее значение высоты
        - new_value: новое значение высоты
        - change: величина изменения
    """
    anomalies = []
    
    # Вычисляем разницу высот между соседними записями
    alt_diff = df['relative_alt'].diff()
    
    # Находим индексы, где абсолютная разница превышает порог
    anomaly_indices = np.where(abs(alt_diff) > threshold)[0]
    
    for idx in anomaly_indices:

        anomaly = {
            'timestamp': df.iloc[idx]['timestamp'],
            'old_value': df.iloc[idx-1]['relative_alt'],
            'new_value': df.iloc[idx]['relative_alt'],
            'change': alt_diff.iloc[idx]
        }
        anomalies.append(anomaly)
    
    return anomalies


def detect_gps_anomalies(df: pd.DataFrame, min_satellites: int = 6) -> List[Dict]:
    """
    Находит потерю GPS сигнала.
    
    Аномалия: количество спутников < min_satellites ИЛИ gps_fix < 3
    
    Args:
        df: DataFrame с данными полёта
        min_satellites: минимальное количество спутников
        
    Returns:
        Список словарей с полями:
        - timestamp: время аномалии
        - satellites: количество спутников
        - fix_type: тип фиксации GPS
    """
    anomalies = []
    
    if df is None:
        return anomalies
    
    # Фильтруем строки с аномалиями
    anomaly_mask = (df['satellites'] < min_satellites) | (df['gps_fix'] < 3)
    anomaly_rows = df[anomaly_mask]
    
    for _, row in anomaly_rows.iterrows():
        anomaly = {
            'timestamp': row['timestamp'],
            'satellites': row['satellites'],
            'fix_type': row['gps_fix']
        }
        anomalies.append(anomaly)
    
    return anomalies


def detect_battery_anomalies(df: pd.DataFrame,
                             min_voltage: float = 10.5,
                             min_percent: int = 20) -> List[Dict]:
    """
    Находит критические уровни батареи.
    
    Аномалия: voltage < min_voltage ИЛИ remaining < min_percent
    
    Args:
        df: DataFrame с данными полёта
        min_voltage: минимальное напряжение в вольтах
        min_percent: минимальный уровень заряда в %
        
    Returns:
        Список словарей с полями:
        - timestamp: время аномалии
        - voltage: напряжение
        - remaining: уровень заряда в %
        - reason: причина ('low_voltage' или 'low_charge')
    """
    anomalies = []
    
    if df is None:
        return anomalies
    
    # Проверяем каждую строку
    for _, row in df.iterrows():
        reasons = []
        
        if row['battery_voltage'] < min_voltage:
            reasons.append('low_voltage')
        
        if row['battery_remaining'] < min_percent:
            reasons.append('low_charge')
        
        # Если есть хотя бы одна причина, добавляем аномалию
        if reasons:
            # Для каждой причины создаём отдельную запись
            for reason in reasons:
                anomaly = {
                    'timestamp': row['timestamp'],
                    'voltage': row['battery_voltage'],
                    'remaining': row['battery_remaining'],
                    'reason': reason
                }
                anomalies.append(anomaly)
    
    return anomalies


def detect_speed_anomalies(df: pd.DataFrame, max_speed: float = 20.0) -> List[Dict]:
    """
    Находит превышение скорости.
    
    Аномалия: groundspeed > max_speed
    
    Args:
        df: DataFrame с данными полёта
        max_speed: максимальная скорость в м/с
        
    Returns:
        Список словарей с полями:
        - timestamp: время аномалии
        - speed: скорость в м/с
    """
    anomalies = []
    
    if df is None:
        return anomalies
    
    # Фильтруем строки с превышением скорости
    speed_mask = df['groundspeed'] > max_speed
    speed_anomalies = df[speed_mask]
    
    for _, row in speed_anomalies.iterrows():
        anomaly = {
            'timestamp': row['timestamp'],
            'speed': row['groundspeed']
        }
        anomalies.append(anomaly)
    
    return anomalies


def detect_attitude_anomalies(df: pd.DataFrame, max_rate: float = 0.5) -> List[Dict]:
    """
    Находит нестабильное положение.
    
    Аномалия: резкое изменение roll или pitch (более max_rate рад за секунду)
    
    Args:
        df: DataFrame с данными полёта
        max_rate: максимальная скорость изменения угла в рад/с
        
    Returns:
        Список словарей с полями:
        - timestamp: время аномалии
        - roll: значение крена
        - pitch: значение тангажа
        - roll_rate: скорость изменения крена
        - pitch_rate: скорость изменения тангажа
    """
    anomalies = []
    
    if df is None or len(df) < 2:
        return anomalies
    
    # Вычисляем скорость изменения углов
    roll_rate = df['roll'].diff().abs()
    pitch_rate = df['pitch'].diff().abs()
    
    # Находим индексы, где скорость изменения превышает порог
    anomaly_indices = np.where((roll_rate > max_rate) | (pitch_rate > max_rate))[0]
    
    for idx in anomaly_indices:
        if idx > 0 and idx < len(df):
            anomaly = {
                'timestamp': df.iloc[idx]['timestamp'],
                'roll': df.iloc[idx]['roll'],
                'pitch': df.iloc[idx]['pitch'],
                'roll_rate': roll_rate.iloc[idx],
                'pitch_rate': pitch_rate.iloc[idx]
            }
            anomalies.append(anomaly)
    
    return anomalies


def generate_report(anomalies: Dict[str, List[Dict]], 
                    df: Optional[pd.DataFrame] = None) -> str:
    """
    Генерирует текстовый отчёт об аномалиях.
    
    Args:
        anomalies: словарь с аномалиями по категориям
        df: DataFrame для дополнительной статистики
        
    Returns:
        Строка с отформатированным отчётом
    """
    lines = ["=== FLIGHT LOG ANALYSIS REPORT ===", ""]
    
    # Общая статистика
    if df is not None:
        total_records = len(df)
        lines.append(f"Total records: {total_records}")
        
        if total_records > 1:
            # Вычисляем длительность полёта
            duration = df['timestamp'].iloc[-1] - df['timestamp'].iloc[0]
            lines.append(f"Flight duration: {duration} seconds")
        else:
            lines.append("Flight duration: N/A")
    else:
        lines.append("Total records: N/A")
        lines.append("Flight duration: N/A")
    
    lines.append("")
    
    # Категории аномалий в нужном порядке
    categories = [
        ('altitude', 'ALTITUDE ANOMALIES'),
        ('gps', 'GPS ANOMALIES'),
        ('battery', 'BATTERY ANOMALIES'),
        ('speed', 'SPEED ANOMALIES'),
        ('attitude', 'ATTITUDE ANOMALIES')
    ]
    
    for key, title in categories:
        anomaly_list = anomalies.get(key, [])
        count = len(anomaly_list)
        
        lines.append(f"--- {title} ({count} found) ---")
        
        if count == 0:
            lines.append("No anomalies detected.")
        else:
            for anomaly in anomaly_list:
                timestamp = anomaly.get('timestamp', 'N/A')
                
                if key == 'altitude':
                    lines.append(
                        f"[{timestamp}] Rapid altitude change: "
                        f"{anomaly['old_value']:.1f}m -> {anomaly['new_value']:.1f}m "
                        f"(Δ{anomaly['change']:.1f}m)"
                    )
                
                elif key == 'gps':
                    lines.append(
                        f"[{timestamp}] GPS signal degraded: "
                        f"{anomaly['satellites']} satellites, fix_type={anomaly['fix_type']}"
                    )
                
                elif key == 'battery':
                    reason_text = "Low voltage" if anomaly['reason'] == 'low_voltage' else "Low charge"
                    lines.append(
                        f"[{timestamp}] Battery {reason_text}: "
                        f"{anomaly['voltage']:.2f}V, {anomaly['remaining']}%"
                    )
                
                elif key == 'speed':
                    lines.append(
                        f"[{timestamp}] Speed exceeded: {anomaly['speed']:.1f} m/s"
                    )
                
                elif key == 'attitude':
                    lines.append(
                        f"[{timestamp}] Unstable attitude: "
                        f"roll={anomaly['roll']:.2f}, pitch={anomaly['pitch']:.2f} "
                        f"(Δroll={anomaly['roll_rate']:.2f}, Δpitch={anomaly['pitch_rate']:.2f})"
                    )
        
        lines.append("")
    
    lines.append("=== END OF REPORT ===")
    
    return "\n".join(lines)


def main():
    """Пример использования анализатора."""
    import os
    
    # Путь к тестовому файлу
    data_dir = os.path.join(os.path.dirname(__file__), '..', 'data')
    log_file = os.path.join(data_dir, 'flight_log.csv')
    
    if not os.path.exists(log_file):
        print(f"Файл {log_file} не найден")
        print("Скопируйте flight_log.csv из common/test-data/")
        return
    
    # Загрузка данных
    df = load_flight_log(log_file)
    
    if df is None:
        print("Ошибка загрузки данных")
        return
    
    print(f"Загружено записей: {len(df)}")
    
    # Обнаружение аномалий
    anomalies = {
        'altitude': detect_altitude_anomalies(df),
        'gps': detect_gps_anomalies(df),
        'battery': detect_battery_anomalies(df),
        'speed': detect_speed_anomalies(df),
        'attitude': detect_attitude_anomalies(df),
    }
    
    # Генерация отчёта
    report = generate_report(anomalies, df)
    print(report)


if __name__ == '__main__':
    main()