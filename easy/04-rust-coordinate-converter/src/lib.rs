//! Библиотека для конвертации координат WGS84 <-> UTM.
//!
//! # Примеры
//!
//! ```
//! use coordinate_converter::{WGS84, UTM};
//!
//! let moscow = WGS84::new(55.7558, 37.6173).unwrap();
//! let utm = moscow.to_utm();
//! println!("UTM: {} {:.0}E {:.0}N", utm.zone, utm.easting, utm.northing);
//! ```

use std::f64::consts::PI;

/// Ошибки при работе с координатами.
#[derive(Debug, Clone, PartialEq)]
pub enum CoordinateError {
    /// Широта вне диапазона [-90, 90]
    InvalidLatitude(f64),
    /// Долгота вне диапазона [-180, 180]
    InvalidLongitude(f64),
    /// Некорректная UTM зона
    InvalidZone(u8),
}

impl std::fmt::Display for CoordinateError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::InvalidLatitude(lat) => write!(f, "Invalid latitude: {}", lat),
            Self::InvalidLongitude(lon) => write!(f, "Invalid longitude: {}", lon),
            Self::InvalidZone(zone) => write!(f, "Invalid UTM zone: {}", zone),
        }
    }
}

impl std::error::Error for CoordinateError {}

/// Полушарие для UTM координат.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Hemisphere {
    North,
    South,
}

/// Координаты в системе WGS84 (широта/долгота).
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct WGS84 {
    /// Широта в градусах (-90..90)
    pub lat: f64,
    /// Долгота в градусах (-180..180)
    pub lon: f64,
}

/// Координаты в системе UTM.
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct UTM {
    /// Координата на восток (метры)
    pub easting: f64,
    /// Координата на север (метры)
    pub northing: f64,
    /// Номер зоны (1-60)
    pub zone: u8,
    /// Полушарие
    pub hemisphere: Hemisphere,
}

// Константы WGS84 эллипсоида
const WGS84_A: f64 = 6378137.0;  // большая полуось (м)
const WGS84_F: f64 = 1.0 / 298.257223563;  // сплюснутость
const WGS84_E: f64 = 0.0818191908426;  // эксцентриситет
const UTM_K0: f64 = 0.9996;  // масштабный коэффициент UTM
const EARTH_RADIUS: f64 = 6371000.0;  // средний радиус Земли (м)

impl WGS84 {
    /// Создать координату WGS84 из градусов.
    ///
    /// # Ошибки
    ///
    /// Возвращает ошибку если координаты вне допустимого диапазона.
    ///
    /// # Примеры
    ///
    /// ```
    /// use coordinate_converter::WGS84;
    ///
    /// let point = WGS84::new(55.7558, 37.6173).unwrap();
    /// assert_eq!(point.lat, 55.7558);
    /// ```
    pub fn new(lat: f64, lon: f64) -> Result<Self, CoordinateError> {
        // TODO: Реализуйте валидацию координат
        //
        // Требования:
        // - lat должна быть в диапазоне [-90, 90]
        // - lon должна быть в диапазоне [-180, 180]
        // - Возвращайте соответствующую ошибку при невалидных значениях
        
        // Ваш код здесь
        
        Ok(Self { lat, lon })
    }
    
    /// Конвертировать в координаты UTM.
    ///
    /// # Примеры
    ///
    /// ```
    /// use coordinate_converter::{WGS84, Hemisphere};
    ///
    /// let moscow = WGS84::new(55.7558, 37.6173).unwrap();
    /// let utm = moscow.to_utm();
    /// assert_eq!(utm.zone, 37);
    /// ```
    pub fn to_utm(&self) -> UTM {
        // TODO: Реализуйте конвертацию WGS84 -> UTM
        //
        // Шаги:
        // 1. Вычислите номер зоны: zone = floor((lon + 180) / 6) + 1
        // 2. Вычислите центральный меридиан зоны
        // 3. Примените формулы проекции Transverse Mercator
        //
        // Для упрощения можно использовать приближённые формулы.
        // Точность ~1м будет достаточна.
        
        // Вычисление зоны
        let zone = ((self.lon + 180.0) / 6.0).floor() as u8 + 1;
        
        // Определение полушария
        let hemisphere = if self.lat >= 0.0 {
            Hemisphere::North
        } else {
            Hemisphere::South
        };
        
        // TODO: Вычислите easting и northing
        // Это упрощённая версия - замените на правильные формулы
        
        let easting = 500000.0;  // TODO: правильное значение
        let northing = 0.0;  // TODO: правильное значение
        
        UTM {
            easting,
            northing,
            zone,
            hemisphere,
        }
    }
    
    /// Вычислить расстояние до другой точки в метрах.
    ///
    /// Использует формулу Хаверсина для расчёта расстояния
    /// по поверхности сферы.
    ///
    /// # Примеры
    ///
    /// ```
    /// use coordinate_converter::WGS84;
    ///
    /// let moscow = WGS84::new(55.7558, 37.6173).unwrap();
    /// let spb = WGS84::new(59.9343, 30.3351).unwrap();
    /// let distance = moscow.distance_to(&spb);
    /// // ~635 км
    /// assert!(distance > 600_000.0 && distance < 700_000.0);
    /// ```
    pub fn distance_to(&self, other: &WGS84) -> f64 {
        // TODO: Реализуйте формулу Хаверсина
        //
        // Формула:
        // a = sin²(Δφ/2) + cos(φ1) * cos(φ2) * sin²(Δλ/2)
        // c = 2 * atan2(√a, √(1-a))
        // d = R * c
        //
        // где φ — широта, λ — долгота в радианах
        // R — радиус Земли (EARTH_RADIUS)
        
        // Ваш код здесь
        
        0.0  // TODO: замените на правильное значение
    }
    
    /// Вычислить азимут (направление) к другой точке.
    ///
    /// Возвращает угол в градусах (0-360), где:
    /// - 0° = север
    /// - 90° = восток
    /// - 180° = юг
    /// - 270° = запад
    ///
    /// # Примеры
    ///
    /// ```
    /// use coordinate_converter::WGS84;
    ///
    /// let moscow = WGS84::new(55.7558, 37.6173).unwrap();
    /// let spb = WGS84::new(59.9343, 30.3351).unwrap();
    /// let bearing = moscow.bearing_to(&spb);
    /// // ~318° (северо-запад)
    /// assert!(bearing > 300.0 && bearing < 340.0);
    /// ```
    pub fn bearing_to(&self, other: &WGS84) -> f64 {
        // TODO: Реализуйте расчёт азимута
        //
        // Формула:
        // θ = atan2(sin(Δλ) * cos(φ2), 
        //           cos(φ1) * sin(φ2) - sin(φ1) * cos(φ2) * cos(Δλ))
        //
        // Результат в радианах, конвертируйте в градусы [0, 360)
        
        // Ваш код здесь
        
        0.0  // TODO: замените на правильное значение
    }
}

impl UTM {
    /// Конвертировать UTM координаты обратно в WGS84.
    ///
    /// # Примеры
    ///
    /// ```
    /// use coordinate_converter::{WGS84, UTM, Hemisphere};
    ///
    /// let utm = UTM {
    ///     easting: 413589.0,
    ///     northing: 6178456.0,
    ///     zone: 37,
    ///     hemisphere: Hemisphere::North,
    /// };
    /// let wgs = utm.to_wgs84();
    /// assert!((wgs.lat - 55.7558).abs() < 0.001);
    /// ```
    pub fn to_wgs84(&self) -> WGS84 {
        // TODO: Реализуйте обратную конвертацию UTM -> WGS84
        //
        // Это обратные формулы для to_utm()
        // Для упрощения можно использовать итеративный метод
        
        // Ваш код здесь
        
        WGS84 { lat: 0.0, lon: 0.0 }  // TODO: замените на правильное значение
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_wgs84_new_valid() {
        let point = WGS84::new(55.7558, 37.6173);
        assert!(point.is_ok());
    }
    
    #[test]
    fn test_wgs84_new_invalid_lat() {
        let point = WGS84::new(91.0, 0.0);
        assert!(matches!(point, Err(CoordinateError::InvalidLatitude(_))));
    }
    
    #[test]
    fn test_wgs84_new_invalid_lon() {
        let point = WGS84::new(0.0, 181.0);
        assert!(matches!(point, Err(CoordinateError::InvalidLongitude(_))));
    }
    
    #[test]
    fn test_utm_zone_calculation() {
        // Москва должна быть в зоне 37
        let moscow = WGS84::new(55.7558, 37.6173).unwrap();
        let utm = moscow.to_utm();
        assert_eq!(utm.zone, 37);
        assert_eq!(utm.hemisphere, Hemisphere::North);
    }
    
    #[test]
    fn test_distance_same_point() {
        let point = WGS84::new(55.7558, 37.6173).unwrap();
        let distance = point.distance_to(&point);
        assert!(distance < 0.01);
    }
    
    #[test]
    fn test_distance_moscow_spb() {
        let moscow = WGS84::new(55.7558, 37.6173).unwrap();
        let spb = WGS84::new(59.9343, 30.3351).unwrap();
        let distance = moscow.distance_to(&spb);
        // Примерно 635 км, допуск 5%
        assert!(distance > 600_000.0);
        assert!(distance < 670_000.0);
    }
    
    #[test]
    fn test_bearing_north() {
        let point1 = WGS84::new(50.0, 0.0).unwrap();
        let point2 = WGS84::new(51.0, 0.0).unwrap();
        let bearing = point1.bearing_to(&point2);
        // Направление на север должно быть ~0°
        assert!(bearing < 1.0 || bearing > 359.0);
    }
    
    #[test]
    fn test_bearing_east() {
        let point1 = WGS84::new(0.0, 0.0).unwrap();
        let point2 = WGS84::new(0.0, 1.0).unwrap();
        let bearing = point1.bearing_to(&point2);
        // Направление на восток должно быть ~90°
        assert!(bearing > 89.0 && bearing < 91.0);
    }
}
