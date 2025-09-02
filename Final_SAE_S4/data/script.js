const button_on_ref = document.getElementById("button_on_id");
const button_off_ref = document.getElementById("button_off_id");
const servo_slider_ref = document.getElementById("servo_slider_id");
const slider_value_ref = document.getElementById("slider_value_id");
const day_ref = document.getElementById("day");
const date_ref = document.getElementById("date");
const time_ref = document.getElementById("time");
const cityinfos_ref = document.getElementById("City");
const button_API_ref = document.getElementById("button_API_id");
const API_temperature_ref = document.getElementById("API_temperature_id");
const API_cloud_ref = document.getElementById("API_cloud_id");
const API_icon_ref = document.getElementById("API_icon_id");
const delay = 1000;

button_on_ref.addEventListener("click", (event) => {
  console.log("On");
  button_on_ref.classList.add("w3-disabled");
  button_off_ref.classList.remove("w3-disabled");
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/button?state=1", true);
  xhttp.send();
});

button_off_ref.addEventListener("click", (event) => {
  console.log("Off");
  button_on_ref.classList.remove("w3-disabled");
  button_off_ref.classList.add("w3-disabled");
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/button?state=0", true);
  xhttp.send();
});

servo_slider_ref.addEventListener("change", (event) => {
  const value = event.target.value;
  console.log("slider value " + value);

  slider_value_ref.textContent = value;

  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/slider?slider_value=" + value, true);
  xhttp.send();
});

/*adress_1 + icon + adress_2 (cpp)
src->img = jsonCity.iconURL*/

button_API_ref.addEventListener("click", (event) => {
  const value = cityinfos_ref.value;
  console.log(value);

  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/cityinfos?cityName=" + value, true);
  xhttp.send();

  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      const jsonCity = JSON.parse(this.responseText);
      console.log(this.responseText);
      API_temperature_ref.textContent = jsonCity.main.temp;

      const iconURL = "https://openweathermap.org/img/wn/" + jsonCity.weather[0].icon + "@2x.png";
      console.log(iconURL);
      API_icon_ref.src = iconURL;
      API_cloud_ref.textContent = jsonCity.weather[0].description;
    }
  }
});

setInterval(() => {
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/datas", true);
  xhttp.send();

  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      const value = JSON.parse(this.responseText);
      console.log(this.responseText);
      gaugeTemperature.refresh(value.temperature);
      gaugeLux.refresh(value.lux);
      gaugeHumidity.refresh(value.humidity);
      gaugePressure.refresh(value.pressure);
    }
  }
}, delay);

const gaugeTemperature = new JustGage({
  id: "gauge_temperature",
  value: 0,
  min: -10,
  max: 50,
  decimals: 2,
  title: "",
  label: "°C",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});
//
const gaugeHumidity = new JustGage({
  id: "gauge_humidity",
  value: 0,
  min: 0,
  max: 100,
  decimals: 2,
  title: "",
  label: "%",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});
//
const gaugePressure = new JustGage({
  id: "gauge_pressure",
  value: 0,
  min: 0,
  max: 1300,
  decimals: 0,
  title: "",
  label: "hPa",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});
//
const gaugeLux = new JustGage({
  id: "gauge_lux",
  value: 0,
  min: 0,
  max: 3000,
  decimals: 0,
  title: "",
  label: "Lux",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});