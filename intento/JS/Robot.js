const localizacion = "../PHP/index.php"

const camera = document.getElementById("camera");
const toggleBtn = document.getElementById("toggle");
const url = "http://192.168.1.31/mjpeg/1";
let streaming = false;

const leftBtn = document.getElementById("left");
const rightBtn = document.getElementById("rigth");
const upBtn = document.getElementById("up");
const downBtn = document.getElementById("dawn");

var posX = 512;
var posY = 512;

async function enviarInfo(){
    try{
        const response = await fetch (localizacion,{
            method: "POST",
  			headers: {
            'Content-Type': 'application/json'
        },
  			body:JSON.stringify({
                action: "joistic",
                plataform: "Web",
                ejeX: posX,
                ejeY: posY
            })
        });
        
        const data = await response.text()
        console.log(data)
        if (data.sucess){
            console.log ("todo ha salido bien");
        }
        else{
            console.log("todo ha salido mal")
        }
    }
    catch (error) {
        console.error("❌ Eror en fetch:", error);
    }
}





leftBtn.addEventListener("click", (event) => {
    ejeX("-");
});
rightBtn.addEventListener("click", (event) => {
    ejeX("+");
});
upBtn.addEventListener("click", (event) => {
    ejey("+");
});
downBtn.addEventListener("click", (event) => {
    ejey("-");
});

function ejeX(eje){
    if (eje == "-"){
        if (posX > 10){
            posX -= 10;
            enviarInfo();
        }
    }
    else if (eje == "+"){
        if (posX < 1013){
            posX += 10;
            enviarInfo();
        }
    }
}
function ejeY(eje){
    if (eje == "-"){
        if (posY > 10){
            posY -= 10;
            enviarInfo();
        }
    }
    else if (eje == "+"){
        if (posY < 1013){
            posY += 10;
            enviarInfo();
        }
    }
}
function startStream() {
    camera.src = url + "?t=" + new Date().getTime(); // evitar cache
    streaming = true;
    toggleBtn.textContent = "Detener stream";
}

function stopStream() {
    camera.src = "";
    streaming = false;
    toggleBtn.textContent = "Reanudar stream";
}

toggleBtn.addEventListener("click", () => {
    if (streaming) stopStream();
    else startStream();
});

// Intentar reconectar si hay error de red
camera.addEventListener("error", () => {
    console.warn("Conexión perdida, intentando reconectar...");
    setTimeout(() => {
    if (streaming) startStream();
    }, 3000);
});

// Iniciar al cargar
//startStream();