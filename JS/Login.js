const container = document.getElementById("container");
const registerBtn = document.getElementById("register");
const loginBtn = document.getElementById("login");
const buttonInicio = document.getElementById("buttonInicio");

registerBtn.addEventListener("click", (e) => {
  e.preventDefault();
  container.classList.add("active");
});

loginBtn.addEventListener("click", (e) => {
  e.preventDefault();
  container.classList.remove("active");
});

buttonInicio.addEventListener("click", (event) => {
  event.preventDefault();
  window.location.href = './Index.html';
})

function mostrarMain(mainAMostrar, mains) {
  window.scrollTo(0, 0);
  styleTag.setAttribute('href', `./Styles/${mainAMostrar}.css`)
  mains.forEach(element => {
    determinarHeader(mainAMostrar)
    if (element.id === `${mainAMostrar}`) {
      element.classList.remove('none')
    } else {
      element.classList.add('none')
    }

    if (mainAMostrar === 'Login') {
      Header.classList.add('none');
      Footer.classList.add('none');
    } else {
      Header.classList.remove('none');
      Footer.classList.remove('none');
    }

    if (mainAMostrar === 'Home') {
      Header.classList.add('none');
    }
  })
}