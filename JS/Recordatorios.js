const inputNombre = document.getElementById("inputNombre");
const inputFecha = document.getElementById("inputFecha");
const inputHora = document.getElementById("inputHora");
const btnAgregar = document.getElementById("btnAgregar");
const lista = document.getElementById("listaRecordatorios");

// Cargar recordatorios guardados al iniciar
document.addEventListener("DOMContentLoaded", cargarRecordatorios);
btnAgregar.addEventListener("click", agregarRecordatorio);

function agregarRecordatorio() {
  const nombre = inputNombre.value.trim();
  const fecha = inputFecha.value;
  const hora = inputHora.value;

  if (nombre === "" || fecha === "" || hora === "") {
    alert("Por favor completa todos los campos antes de agregar un recordatorio.");
    return;
  }

  const nuevo = { nombre, fecha, hora };
  const recordatorios = obtenerRecordatorios();
  recordatorios.push(nuevo);
  localStorage.setItem("recordatorios", JSON.stringify(recordatorios));

  mostrarRecordatorio(nuevo);
  inputNombre.value = "";
  inputFecha.value = "";
  inputHora.value = "";
}

function mostrarRecordatorio({ nombre, fecha, hora }) {
  const div = document.createElement("div");
  div.classList.add("Recordatorios__lista-item");

  const header = document.createElement("div");
  header.classList.add("Recordatorios__lista-header");

  const nombreElem = document.createElement("p");
  nombreElem.classList.add("Recordatorios__lista-texto");
  nombreElem.textContent = nombre;

  const borrar = document.createElement("i");
  borrar.classList.add("fa-solid", "fa-trash", "Recordatorios__lista-borrar");
  borrar.addEventListener("click", () => eliminarRecordatorio(nombre, div));

  header.appendChild(nombreElem);
  header.appendChild(borrar);

  const fechaHora = document.createElement("p");
  fechaHora.classList.add("Recordatorios__lista-fechaHora");
  fechaHora.textContent = `📅 ${formatearFecha(fecha)} - ⏰ ${hora}`;

  div.appendChild(header);
  div.appendChild(fechaHora);
  lista.appendChild(div);
}

function eliminarRecordatorio(nombre, elemento) {
  let recordatorios = obtenerRecordatorios();
  recordatorios = recordatorios.filter(r => r.nombre !== nombre);
  localStorage.setItem("recordatorios", JSON.stringify(recordatorios));
  elemento.remove();
}

function cargarRecordatorios() {
  const recordatorios = obtenerRecordatorios();
  recordatorios.forEach(mostrarRecordatorio);
}

function obtenerRecordatorios() {
  return JSON.parse(localStorage.getItem("recordatorios")) || [];
}

function formatearFecha(fechaISO) {
  const fecha = new Date(fechaISO);
  return `${fecha.getDate().toString().padStart(2, "0")}/${(fecha.getMonth() + 1)
    .toString()
    .padStart(2, "0")}/${fecha.getFullYear()}`;
}
