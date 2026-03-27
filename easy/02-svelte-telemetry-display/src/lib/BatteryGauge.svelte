<script lang="ts">
  
  export let battery: number;
  export let color: string;
  
  $: roundedBattery = Math.round(battery);
</script>

  <div class="gauge battery" style="--battery-color: {color}">
    <span class="label">BATTERY</span>
    <span class="value">{battery.toFixed(1)}%</span>
    <div class="bar-background">
      <div class="bar" style="--bar-width: {battery}%"></div>
    </div>
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

    .battery {
    color: var(--battery-color);
  }

  .bar-background {
    height: 100%;
    width: 100%;
  }
  
  .bar {
    height: 10px;
    width:  var(--bar-width);
    background-color: #fff;
  }

</style>