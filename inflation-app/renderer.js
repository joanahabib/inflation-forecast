const countries = [
  { iso: "BG", name: "Bulgaria" },
  { iso: "US", name: "USA" },
  { iso: "DE", name: "Germany" },
  { iso: "JP", name: "Japan" },
  { iso: "UK", name: "United Kingdom"},
  { iso: "FR", name: "France"},
  { iso: "IT", name: "Italy"},
  { iso: "SP", name: "Spain"},
  { iso: "QT", name: "Qatar"},
  { iso: "RU", name: "Russia"},
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
  { iso: "", name: },
];

const select = document.getElementById("country-select");
const runBtn = document.getElementById("run-btn");

countries.forEach(c => {
  const option = document.createElement("option");
  option.value = c.iso;
  option.text = c.name;
  select.appendChild(option);
});

const ctx = document.getElementById("chart").getContext("2d");

const chart = new Chart(ctx, {
  type: "line",
  data: {
    labels: [],
    datasets: [
      {
        label: "ARX Forecast",
        data: [],
        borderColor: "#3b82f6",
        borderWidth: 2,
        tension: 0.4
      },
      {
        label: "Kalman Smoothed",
        data: [],
        borderColor: "#22c55e",
        borderWidth: 2,
        tension: 0.4
      }
    ]
  },
  options: {
    plugins: {
      legend: {
        labels: {
          color: "#e2e8f0"
        }
      }
    },
    scales: {
      x: {
        ticks: { color: "#94a3b8" }
      },
      y: {
        ticks: { color: "#94a3b8" }
      }
    }
  }
});

runBtn.onclick = async () => {
  const iso = select.value;

  const result = await window.api.runModel(iso);

  console.log(result);

  const arx = (result.forecast || []).map(Number);
  const kalman = (result.kalman || result.smoothed || []).map(Number);

  if (!arx.length && !kalman.length) {
    alert("No data returned");
    return;
  }

  const base = arx.length ? arx : kalman;

  chart.data.labels = base.map((_, i) => "t+" + (i + 1));

  chart.data.datasets[0].data = arx;
  chart.data.datasets[1].data = kalman;

  chart.update();

  const finalData = kalman.length ? kalman : arx;

  document.getElementById("current").innerText =
    Number(finalData[finalData.length - 1]).toFixed(2);

  const trendEl = document.getElementById("trend");

  if (finalData[finalData.length - 1] > finalData[0]) {
    trendEl.innerText = "📈 Up";
    trendEl.style.color = "#22c55e";
  } else {
    trendEl.innerText = "📉 Down";
    trendEl.style.color = "#ef4444";
  }
};
