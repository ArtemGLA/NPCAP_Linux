<script lang="ts">
  /**
   * DroneMap - интерактивная карта с отображением дрона и траектории.
   * 
   * TODO: Реализуйте компонент карты
   * 
   * Требования:
   * 1. Инициализация Leaflet карты при монтировании
   * 2. Подключение к WebSocket для получения позиции
   * 3. Отображение маркера дрона с вращением по heading
   * 4. Рисование траектории (polyline)
   * 5. Отображение waypoints миссии
   * 6. Функция центрирования на дроне
   */
  
  import { onMount, onDestroy } from 'svelte';
  import type { DronePosition, Mission, MapConfig, WebSocketMessage } from './types';
  import { DEFAULT_MAP_CONFIG } from './types';
  
  export let config: MapConfig = DEFAULT_MAP_CONFIG;
  export let mission: Mission | null = null;
  export let wsUrl: string = 'ws://localhost:8080';
  export let followDrone: boolean = true;
  
  let mapContainer: HTMLDivElement;
  let map: any = null;  // L.Map
  let droneMarker: any = null;  // L.Marker
  let trajectoryLine: any = null;  // L.Polyline
  let waypointMarkers: any[] = [];
  
  let dronePosition: DronePosition | null = null;
  let trajectory: [number, number][] = [];
  let ws: WebSocket | null = null;
  let connected: boolean = false;
  
  const MAX_TRAJECTORY_POINTS = 1000;
  
  onMount(async () => {
    // TODO: Динамический импорт Leaflet
    // const L = await import('leaflet');
    
    // TODO: Инициализация карты
    // map = L.map(mapContainer).setView(config.center, config.zoom);
    // L.tileLayer(config.tileUrl).addTo(map);
    
    // TODO: Создание маркера дрона
    // droneMarker = L.marker(config.center, { icon: droneIcon }).addTo(map);
    
    // TODO: Создание polyline для траектории
    // trajectoryLine = L.polyline([], { color: 'blue' }).addTo(map);
    
    // TODO: Подключение к WebSocket
    // connectWebSocket();
  });
  
  onDestroy(() => {
    // TODO: Очистка ресурсов
    // ws?.close();
    // map?.remove();
  });
  
  function connectWebSocket() {
    // TODO: Реализуйте WebSocket подключение
    //
    // Шаги:
    // 1. Создать WebSocket соединение
    // 2. Обработать onopen, onmessage, onclose, onerror
    // 3. При получении 'position' - обновить dronePosition
    // 4. При получении 'mission' - обновить waypoints
    // 5. Реализовать переподключение при разрыве
    
    // Ваш код здесь
  }
  
  function updateDronePosition(pos: DronePosition) {
    // TODO: Обновить позицию дрона
    //
    // Шаги:
    // 1. Обновить dronePosition
    // 2. Переместить маркер: droneMarker.setLatLng([pos.lat, pos.lon])
    // 3. Повернуть маркер согласно heading
    // 4. Добавить точку в траекторию
    // 5. Обновить polyline
    // 6. Если followDrone - центрировать карту
    
    dronePosition = pos;
    
    // Добавить точку в траекторию
    trajectory = [...trajectory, [pos.lat, pos.lon]];
    if (trajectory.length > MAX_TRAJECTORY_POINTS) {
      trajectory = trajectory.slice(-MAX_TRAJECTORY_POINTS);
    }
    
    // Ваш код здесь
  }
  
  function updateWaypoints(m: Mission) {
    // TODO: Обновить маркеры waypoints
    //
    // Шаги:
    // 1. Удалить старые маркеры
    // 2. Создать новые маркеры для каждого waypoint
    // 3. Нарисовать линии между waypoints
    
    // Ваш код здесь
  }
  
  function centerOnDrone() {
    if (dronePosition && map) {
      map.setView([dronePosition.lat, dronePosition.lon], config.zoom);
    }
  }
  
  function toggleFollow() {
    followDrone = !followDrone;
  }
</script>

<div class="drone-map">
  <div class="map-container" bind:this={mapContainer}></div>
  
  <div class="controls">
    <button on:click={centerOnDrone} title="Center on drone">
      ⌖
    </button>
    <button on:click={toggleFollow} class:active={followDrone} title="Follow drone">
      {followDrone ? '🔒' : '🔓'}
    </button>
  </div>
  
  <div class="status-bar">
    <span class="connection" class:connected>
      {connected ? '● Connected' : '○ Disconnected'}
    </span>
    
    {#if dronePosition}
      <span>Lat: {dronePosition.lat.toFixed(4)}</span>
      <span>Lon: {dronePosition.lon.toFixed(4)}</span>
      <span>Alt: {dronePosition.alt.toFixed(1)}m</span>
      <span>Speed: {dronePosition.groundspeed.toFixed(1)}m/s</span>
    {/if}
  </div>
</div>

<style>
  .drone-map {
    position: relative;
    width: 100%;
    height: 100%;
    min-height: 400px;
  }
  
  .map-container {
    width: 100%;
    height: 100%;
    background: #e0e0e0;
  }
  
  .controls {
    position: absolute;
    top: 10px;
    right: 10px;
    z-index: 1000;
    display: flex;
    flex-direction: column;
    gap: 5px;
  }
  
  .controls button {
    width: 40px;
    height: 40px;
    border: none;
    border-radius: 4px;
    background: white;
    box-shadow: 0 2px 6px rgba(0, 0, 0, 0.3);
    cursor: pointer;
    font-size: 1.2rem;
  }
  
  .controls button:hover {
    background: #f0f0f0;
  }
  
  .controls button.active {
    background: #4ade80;
  }
  
  .status-bar {
    position: absolute;
    bottom: 0;
    left: 0;
    right: 0;
    padding: 8px 12px;
    background: rgba(0, 0, 0, 0.7);
    color: white;
    font-size: 0.875rem;
    display: flex;
    gap: 1rem;
    z-index: 1000;
  }
  
  .connection {
    color: #f87171;
  }
  
  .connection.connected {
    color: #4ade80;
  }
</style>
