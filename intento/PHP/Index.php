<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With");
header("Content-Type: application/json; charset=utf-8");

$method = $_SERVER['REQUEST_METHOD'];
$input = json_decode(file_get_contents("php://input"), true);
$action = $_GET['action'] ?? ($input['action'] ?? null);
$plataform = $_GET['plataform'] ?? ($input['plataform'] ?? null);

require_once 'PoscicionBD.php';
require_once 'Conecciones.php';
if ($method === 'OPTIONS') {
    http_response_code(204);
    exit;
}
if (!$action) {
    echo json_encode(["error" => "No se especificó acción"]);
    exit;
}
switch ($method){
    case 'GET':
        if ($plataform === "Web"){
            
        }
        if ($plataform === "Robot"){
            if ($action === "joistic"){
                $bd = new PoscicionBD($pdo);
                $bd->obtenerPos();
            }
        }
        break;
    case 'POST':
        
        if ($plataform === "Web"){
            if ($action === "joistic"){

                $bd = new PoscicionBD($pdo);
                $bd->actualizarPos($input['ejeX'], $input['ejeY']);
            }
        }
        if ($plataform === "Robot"){
            
        }
        break;
    default:
        echo json_encode(["error" => "Método no permitido"]);
        break;
}


?>