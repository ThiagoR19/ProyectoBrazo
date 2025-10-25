const Recortorios = document.querySelector('.Recordatorios')
const Editar = document.querySelector('.Hero__header-icon')
const titulo = document.querySelector('.titulo')

Editar.addEventListener('click', () => {
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