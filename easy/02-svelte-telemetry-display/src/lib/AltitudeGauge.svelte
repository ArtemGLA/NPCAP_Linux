<script lang="ts">

  export let altitude: number;
  export let color: string;

  let lastAltitude = altitude;
  let arrow = ' ';
  
  $: {
    arrow = altitude > lastAltitude ? '⬆' : altitude < lastAltitude ? '⬇' : '•';
    lastAltitude = altitude;
  }
  
  // Реактивные вычисления
  $: formattedAltitude = altitude.toFixed(0);
</script>

<div class="gauge altitude" style="--gauge-color: {color}">
  <span class="label">ALTITUDE</span>
  <span class="value">{formattedAltitude} м</span>
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
  
  
  .altitude {
    color: var(--gauge-color);
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