//! Основная логика маршрутизатора.
//!
//! TODO: Реализуйте MAVLink роутер.

use crate::config::{Config, FilterConfig};
use crate::mavlink::MAVLinkMessage;
use std::collections::HashMap;
use std::net::SocketAddr;
use std::sync::Arc;
use tokio::net::UdpSocket;
use tokio::sync::RwLock;

/// Входной канал.
pub struct Input {
    pub name: String,
    pub socket: Arc<UdpSocket>,
    pub system_id: Option<u8>,
}

/// Выходной канал.
pub struct Output {
    pub name: String,
    pub target: SocketAddr,
    pub socket: Arc<UdpSocket>,
}

/// Правило маршрутизации.
pub struct Route {
    pub from: String,
    pub to: Vec<String>,
    pub filter: FilterConfig,
}

/// Статистика роутера.
#[derive(Default)]
pub struct RouterStats {
    pub packets_received: u64,
    pub packets_routed: u64,
    pub packets_dropped: u64,
}

/// MAVLink Router.
pub struct Router {
    inputs: Vec<Input>,
    outputs: HashMap<String, Output>,
    routes: Vec<Route>,
    stats: Arc<RwLock<RouterStats>>,
}

impl Router {
    /// Создать роутер из конфигурации.
    ///
    /// TODO: Реализуйте создание роутера
    ///
    /// Шаги:
    /// 1. Для каждого input из конфига создать UDP сокет (UdpSocket::bind)
    /// 2. Для каждого output создать UDP сокет для отправки
    /// 3. Создать список Route из конфига
    pub async fn new(config: Config) -> Result<Self, Box<dyn std::error::Error>> {
        let inputs = Vec::new();
        let outputs = HashMap::new();
        let routes = Vec::new();
        
        // TODO: Создайте UDP сокеты для inputs
        // for input_cfg in &config.inputs {
        //     let socket = UdpSocket::bind(&input_cfg.bind).await?;
        //     inputs.push(Input {
        //         name: input_cfg.name.clone(),
        //         socket: Arc::new(socket),
        //         system_id: input_cfg.system_id,
        //     });
        // }
        
        // TODO: Создайте UDP сокеты для outputs
        // for output_cfg in &config.outputs {
        //     ...
        // }
        
        // TODO: Создайте правила маршрутизации
        // for route_cfg in &config.routes {
        //     ...
        // }
        
        Ok(Self {
            inputs,
            outputs,
            routes,
            stats: Arc::new(RwLock::new(RouterStats::default())),
        })
    }
    
    /// Запустить роутер.
    ///
    /// TODO: Реализуйте основной цикл обработки пакетов
    ///
    /// Подсказка: используйте tokio::select! для обработки нескольких
    /// входных сокетов одновременно:
    ///
    /// ```ignore
    /// loop {
    ///     tokio::select! {
    ///         result = inputs[0].socket.recv_from(&mut buf) => {
    ///             let (len, addr) = result?;
    ///             self.handle_packet(&inputs[0].name, &buf[..len]).await?;
    ///         }
    ///         // ... другие inputs
    ///     }
    /// }
    /// ```
    pub async fn run(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        tracing::info!("Router starting...");
        
        // TODO: Реализуйте цикл приёма и маршрутизации пакетов
        
        // Пока просто ждём Ctrl+C
        tokio::signal::ctrl_c().await?;
        
        Ok(())
    }
    
    /// Обработать входящий пакет.
    ///
    /// TODO: Реализуйте обработку пакета
    ///
    /// Шаги:
    /// 1. Распарсить MAVLink сообщение
    /// 2. Обновить статистику packets_received
    /// 3. Найти подходящие маршруты через route()
    /// 4. Отправить пакет каждому получателю
    /// 5. Обновить статистику packets_routed
    async fn handle_packet(
        &self,
        input_name: &str,
        data: &[u8],
    ) -> Result<(), Box<dyn std::error::Error>> {
        // Ваш код здесь
        Ok(())
    }
    
    /// Найти выходные каналы для сообщения.
    ///
    /// TODO: Реализуйте логику маршрутизации
    ///
    /// Для каждого route проверьте:
    /// 1. Совпадает ли route.from с input_name
    /// 2. Проходит ли сообщение фильтр (matches_filter)
    /// 3. Если да — добавьте все outputs из route.to в результат
    fn route(&self, input_name: &str, msg: &MAVLinkMessage) -> Vec<&Output> {
        // Ваш код здесь
        Vec::new()
    }
    
    /// Проверить соответствие сообщения фильтру.
    ///
    /// TODO: Реализуйте проверку фильтра
    ///
    /// Проверьте:
    /// 1. Если filter.message_ids задан — msg.message_id должен быть в списке
    /// 2. Если filter.system_ids задан — msg.system_id должен быть в списке
    /// 3. Если оба не заданы — пропускать всё
    fn matches_filter(&self, msg: &MAVLinkMessage, filter: &FilterConfig) -> bool {
        // Ваш код здесь
        true
    }
    
    /// Получить статистику.
    pub async fn get_stats(&self) -> RouterStats {
        self.stats.read().await.clone()
    }
}

impl Clone for RouterStats {
    fn clone(&self) -> Self {
        Self {
            packets_received: self.packets_received,
            packets_routed: self.packets_routed,
            packets_dropped: self.packets_dropped,
        }
    }
}
