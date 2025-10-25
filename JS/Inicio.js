const menu = document.getElementById('menu-toggle');
const sidebar = document.getElementById('sidebar');
const closeBtn = sidebar.querySelector('.fa-xmark');
const Recortorios = document.querySelector('.Recordatorios')
const Editar = document.querySelector('.Hero__header-icon')
const titulo = document.querySelector('.titulo')

Editar.addEventListener('click', () => {
  // Crear un input con el texto actual
  const input = document.createElement('input');
  input.type = 'text';
  input.value = titulo.textContent;

  titulo.replaceWith(input);
  input.focus();

  input.addEventListener('input', () => {
    titulo.textContent = input.value;
  });

  input.addEventListener('keydown', (e) => {
    if (e.key === 'Enter') {
      titulo.textContent = input.value;
      input.replaceWith(titulo);
    }
  });
});

menu.addEventListener('click', () => {
  if (window.innerWidth <= 900) {
    sidebar.classList.toggle('show');
  } else {
    sidebar.classList.toggle('hide');
  }
});

closeBtn.addEventListener('click', () => {
  sidebar.classList.remove('show');
  sidebar.classList.add('hide');
});