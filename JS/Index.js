document.querySelectorAll('.Hero__article-div').forEach((e)=> {
  e.addEventListener('click', ()=> {
    window.location.href = "Robot.html";
  })
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