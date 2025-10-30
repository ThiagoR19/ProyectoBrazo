const camera = document.getElementById("camera");
    const toggleBtn = document.getElementById("toggle");
    const url = "http://192.168.1.31/mjpeg/1";
    let streaming = true;

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
    startStream();