//! MAVLink парсер и структуры.

use thiserror::Error;

#[derive(Error, Debug)]
pub enum MAVLinkError {
    #[error("Invalid magic byte: expected 0xFE, got {0:#x}")]
    InvalidMagic(u8),
    
    #[error("Packet too short: need {expected}, got {actual}")]
    TooShort { expected: usize, actual: usize },
    
    #[error("Invalid CRC: expected {expected:#x}, got {actual:#x}")]
    InvalidCRC { expected: u16, actual: u16 },
}

/// MAVLink v1 сообщение.
#[derive(Debug, Clone)]
pub struct MAVLinkMessage {
    pub magic: u8,
    pub length: u8,
    pub sequence: u8,
    pub system_id: u8,
    pub component_id: u8,
    pub message_id: u8,
    pub payload: Vec<u8>,
    pub crc: u16,
}

/// CRC extra значения для известных сообщений.
const CRC_EXTRA: &[(u8, u8)] = &[
    (0, 50),    // HEARTBEAT
    (1, 124),   // SYS_STATUS
    (24, 24),   // GPS_RAW_INT
    (30, 39),   // ATTITUDE
    (33, 104),  // GLOBAL_POSITION_INT
    (74, 20),   // VFR_HUD
];

fn get_crc_extra(message_id: u8) -> Option<u8> {
    CRC_EXTRA.iter()
        .find(|(id, _)| *id == message_id)
        .map(|(_, extra)| *extra)
}

fn crc_accumulate(byte: u8, crc: u16) -> u16 {
    let tmp = byte ^ (crc as u8);
    let tmp = tmp ^ (tmp << 4);
    let tmp = tmp as u16;
    (crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4)
}

fn crc_calculate(data: &[u8], crc_extra: u8) -> u16 {
    let mut crc: u16 = 0xFFFF;
    for byte in data {
        crc = crc_accumulate(*byte, crc);
    }
    crc = crc_accumulate(crc_extra, crc);
    crc
}

impl MAVLinkMessage {
    /// Парсить MAVLink пакет из байтов.
    /// 
    /// TODO: Реализуйте парсинг MAVLink пакета
    /// 
    /// Формат MAVLink v1:
    /// - Байт 0: magic (0xFE)
    /// - Байт 1: length (длина payload)
    /// - Байт 2: sequence
    /// - Байт 3: system_id
    /// - Байт 4: component_id
    /// - Байт 5: message_id
    /// - Байты 6..6+length: payload
    /// - Байты 6+length..8+length: CRC (little-endian)
    pub fn parse(data: &[u8]) -> Result<Self, MAVLinkError> {
        // Проверка минимальной длины
        if data.len() < 8 {
            return Err(MAVLinkError::TooShort {
                expected: 8,
                actual: data.len(),
            });
        }
        
        // TODO: Проверьте magic byte
        // TODO: Извлеките заголовок
        // TODO: Проверьте что данных достаточно
        // TODO: Извлеките payload и CRC
        
        // Ваш код здесь
        
        Err(MAVLinkError::TooShort { expected: 0, actual: 0 })
    }
    
    /// Проверить CRC сообщения.
    /// 
    /// TODO: Реализуйте проверку CRC
    pub fn validate_crc(&self) -> bool {
        // TODO: Вычислите CRC для header + payload
        // TODO: Сравните с self.crc
        
        // Ваш код здесь
        
        false
    }
    
    /// Сериализовать сообщение обратно в байты.
    pub fn to_bytes(&self) -> Vec<u8> {
        let mut bytes = Vec::with_capacity(8 + self.payload.len());
        
        bytes.push(self.magic);
        bytes.push(self.length);
        bytes.push(self.sequence);
        bytes.push(self.system_id);
        bytes.push(self.component_id);
        bytes.push(self.message_id);
        bytes.extend_from_slice(&self.payload);
        bytes.push((self.crc & 0xFF) as u8);
        bytes.push((self.crc >> 8) as u8);
        
        bytes
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_crc_accumulate() {
        let crc = crc_accumulate(0x01, 0xFFFF);
        assert_ne!(crc, 0xFFFF);
    }
    
    #[test]
    fn test_parse_too_short() {
        let data = [0xFE, 0x00];
        let result = MAVLinkMessage::parse(&data);
        assert!(matches!(result, Err(MAVLinkError::TooShort { .. })));
    }
    
    #[test]
    fn test_to_bytes() {
        let msg = MAVLinkMessage {
            magic: 0xFE,
            length: 2,
            sequence: 0,
            system_id: 1,
            component_id: 1,
            message_id: 0,
            payload: vec![0x01, 0x02],
            crc: 0xABCD,
        };
        
        let bytes = msg.to_bytes();
        assert_eq!(bytes.len(), 10);
        assert_eq!(bytes[0], 0xFE);
        assert_eq!(bytes[8], 0xCD);  // CRC low byte
        assert_eq!(bytes[9], 0xAB);  // CRC high byte
    }
}
