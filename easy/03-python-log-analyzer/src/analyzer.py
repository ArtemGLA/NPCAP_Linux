"""
Flight Log Analyzer - анализатор полётных логов для обнаружения аномалий.

Задание: реализуйте функции для анализа данных полёта
и обнаружения различных типов аномалий.
"""

import pandas as pd
from typing import Any


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
    # TODO: Реализуйте загрузку CSV файла
    # Подсказка: используйте pd.read_csv()
    
    df = pd.read_csv(filepath)
    return df
    
    pass


def detect_altitude_anomalies(df: pd.DataFrame, threshold: float = 10.0) -> list[dict]:
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
    # TODO: Реализуйте обнаружение аномалий высоты
    #
    # Подсказка:
    # 1. Используйте df['relative_alt'].diff() для вычисления разницы
    # 2. Найдите строки где abs(diff) > threshold
    # 3. Для каждой аномалии создайте словарь с нужными полями
    
    anomalies = []
    
    # Ваш код здесь
    
    return anomalies


def detect_gps_anomalies(df: pd.DataFrame, min_satellites: int = 6) -> list[dict]:
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
    # TODO: Реализуйте обнаружение GPS аномалий
    #
    # Подсказка:
    # 1. Фильтруйте строки: (satellites < min_satellites) | (gps_fix < 3)
    # 2. Для каждой строки создайте словарь с нужными полями
    
    anomalies = []
    
    # Ваш код здесь
    
    return anomalies


def detect_battery_anomalies(df: pd.DataFrame,
                             min_voltage: float = 10.5,
                             min_percent: int = 20) -> list[dict]:
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
    # TODO: Реализуйте обнаружение аномалий батареи
    
    anomalies = []
    
    # Ваш код здесь
    
    return anomalies


def detect_speed_anomalies(df: pd.DataFrame, max_speed: float = 20.0) -> list[dict]:
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
    # TODO: Реализуйте обнаружение превышения скорости
    
    anomalies = []
    
    # Ваш код здесь
    
    return anomalies


def detect_attitude_anomalies(df: pd.DataFrame, max_rate: float = 0.5) -> list[dict]:
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
    # TODO: Реализуйте обнаружение нестабильного положения
    #
    # Подсказка:
    # 1. Вычислите diff() для roll и pitch
    # 2. Найдите строки где abs(roll_diff) > max_rate ИЛИ abs(pitch_diff) > max_rate
    
    anomalies = []
    
    # Ваш код здесь
    
    return anomalies


def generate_report(anomalies: dict[str, list[dict]], 
                    df: pd.DataFrame | None = None) -> str:
    """
    Генерирует текстовый отчёт об аномалиях.
    
    Args:
        anomalies: словарь с аномалиями по категориям
        df: DataFrame для дополнительной статистики
        
    Returns:
        Строка с отформатированным отчётом
    """
    # TODO: Реализуйте генерацию отчёта
    #
    # Формат:
    # === FLIGHT LOG ANALYSIS REPORT ===
    # 
    # Total records: N
    # Flight duration: M seconds
    # 
    # --- ALTITUDE ANOMALIES (X found) ---
    # [timestamp] Description...
    # 
    # ... остальные категории ...
    #
    # === END OF REPORT ===
    
    lines = ["=== FLIGHT LOG ANALYSIS REPORT ===", ""]
    
    # Ваш код здесь
    
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
