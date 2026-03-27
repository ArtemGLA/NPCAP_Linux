//! Integration tests for coordinate converter.

use coordinate_converter::{WGS84, UTM, Hemisphere, CoordinateError};

#[test]
fn test_wgs84_creation() {
    let coord = WGS84::new(55.7558, 37.6173).unwrap();
    assert!((coord.lat - 55.7558).abs() < 1e-6);
    assert!((coord.lon - 37.6173).abs() < 1e-6);
}

#[test]
fn test_wgs84_invalid_lat() {
    let result = WGS84::new(95.0, 37.6173);
    assert!(result.is_err());
}

#[test]
fn test_wgs84_invalid_lon() {
    let result = WGS84::new(55.0, 200.0);
    assert!(result.is_err());
}

#[test]
fn test_distance_same_point() {
    let coord1 = WGS84::new(55.7558, 37.6173).unwrap();
    let coord2 = WGS84::new(55.7558, 37.6173).unwrap();
    
    let distance = coord1.distance_to(&coord2);
    assert!(distance < 1.0); // Less than 1 meter
}

#[test]
fn test_distance_known_points() {
    // Moscow to Saint Petersburg: ~634 km
    let moscow = WGS84::new(55.7558, 37.6173).unwrap();
    let spb = WGS84::new(59.9343, 30.3351).unwrap();
    
    let distance = moscow.distance_to(&spb);
    assert!(distance > 600_000.0 && distance < 700_000.0);
}

#[test]
fn test_bearing_north() {
    let point1 = WGS84::new(55.0, 37.0).unwrap();
    let point2 = WGS84::new(56.0, 37.0).unwrap(); // Due north
    
    let bearing = point1.bearing_to(&point2);
    assert!(bearing < 5.0 || bearing > 355.0); // Near 0 degrees
}

#[test]
fn test_bearing_east() {
    let point1 = WGS84::new(55.0, 37.0).unwrap();
    let point2 = WGS84::new(55.0, 38.0).unwrap(); // Due east
    
    let bearing = point1.bearing_to(&point2);
    assert!(bearing > 85.0 && bearing < 95.0); // Near 90 degrees
}

#[test]
fn test_utm_zone_moscow() {
    let moscow = WGS84::new(55.7558, 37.6173).unwrap();
    let utm = moscow.to_utm().unwrap();
    
    assert_eq!(utm.zone, 37);
    assert_eq!(utm.hemisphere, Hemisphere::North);
}

#[test]
fn test_utm_roundtrip() {
    let original = WGS84::new(55.7558, 37.6173).unwrap();
    let utm = original.to_utm().unwrap();
    let converted = utm.to_wgs84().unwrap();
    
    assert!((original.lat - converted.lat).abs() < 0.0001);
    assert!((original.lon - converted.lon).abs() < 0.0001);
}
