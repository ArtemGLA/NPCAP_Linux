<script lang="ts">
  export let speed: number;
  
  // Определяем направление стрелки
  let lastSpeed = speed;
  let arrow = ' ';
  
  $: {
    arrow = speed > lastSpeed ? '➡' : speed < lastSpeed ? '⬅' : '•';
    lastSpeed = speed;
  }

  $: formattedSpeed = speed.toFixed(1);
</script>

<div class="gauge speed">
  <span class="label">SPEED</span>
  <span class="value">{formattedSpeed} м/с</span>
  <span class="arrow">{arrow}</span>
</div>

<style>
  .gauge {
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 1rem;
    background: var(--bg-tertiary, #16213e);
    border-radius: 8px;
    min-width: 100px;
  }
  
  .label {
    font-size: 0.75rem;
    color: var(--text-secondary, #888);
    margin-bottom: 0.5rem;
  }
  
  .value {
    font-size: 1.5rem;
    font-weight: bold;
  }
</style>