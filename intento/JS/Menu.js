const menu = document.getElementById('menu-toggle');
const sidebar = document.getElementById('sidebar');
const closeBtn = sidebar.querySelector('.fa-xmark');

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