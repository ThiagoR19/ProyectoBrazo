<?php
class PoscicionBD{
    private $pdo;
    private $posicionX;
    private $posicionY;

    public function __construct($pdo){
        $this->pdo = $pdo;
    }

    public function actualizarPos($x, $y){
        try {
        $sql = "UPDATE BrazoPosicion SET ValorX = :ejeX, ValorY = :ejeY WHERE id = 1;";
        $stmt = $this->pdo->prepare($sql);
        $stmt->execute([
            ':ejeX' => $x,
            ':ejeY' => $y
        ]);
        
        echo json_encode([
            "status" => "ok",
            "message" => "Datos subidos correctamente"
        ]);
    } catch (PDOException $e) {
        echo json_encode([
            "status" => "error",
            "message" => $e->getMessage()
        ]);
    }
    }
    public function obtenerPos(){
        try {
            $sql = "SELECT ValorX, ValorY FROM BrazoPosicion WHERE id = 1;";
                $stmt = $this->pdo->prepare($sql);
                $stmt->execute();
                $result = $stmt->fetch(PDO::FETCH_ASSOC);

                if ($result) {
                    echo json_encode([
                        "success" => true,
                        "posX" => (int)$result["ValorX"],
                        "posY" => (int)$result["ValorY"]
                    ]);
                } 
                else {
                    echo json_encode([
                        "success" => false,
                        "posX" => null,
                        "posY" => null
                    ]);
                }
        } 
        catch (PDOException $e) {
            echo json_encode([
                "status" => "error",
                "message" => $e->getMessage()
            ]);
        }
    }
}

?>