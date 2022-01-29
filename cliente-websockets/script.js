var topic;
var client;

// função para abrir a conexão

function abrir_conexao() {
    console.log("conectando...");
    var mqttHost = 'broker.mqttdashboard.com';
    topic = 'lucasfr_uff_si_redes2/esp32/dht/temperature';
    client = new Paho.MQTT.Client(mqttHost, 8000, "myclientid_" + parseInt(Math.random() * 100, 10));

    client.onMessageArrived = onMessageArrived;

    client.connect({ onSuccess: conectar });
}

// comportamento do botão de conectar/desconectar

function clickBotaoConexao() {
    const botao = document.getElementById("botaoConexao");

    if (botao.classList.contains("btn-success")){
        abrir_conexao();
    } else if (botao.classList.contains("btn-danger")) {
        desconectar();
    }
}

// função de conexão

function conectar() {
    console.log("conectado!");
    document.getElementById("conexao").removeAttribute("class", "invisible");
    document.getElementById("conexao").setAttribute("class", "visible p-2 bd-highlight mx-2 my-2 alert alert-success");
    document.getElementById("botaoConexao").removeAttribute("class", "btn-success");
    document.getElementById("botaoConexao").setAttribute("class", "btn btn-danger");
    document.getElementById("botaoConexao").textContent = "Desconectar!";
    client.subscribe(topic);
}

// função que trata as mensagens recebidas

function onMessageArrived(message) {
    console.log("onMessageArrived:" + message.payloadString);
    var msgHtml = `<tr><td>${new Date().toLocaleTimeString()}</td><td>${message.payloadString}</td></tr>`;
    
    document.getElementById("tabela-temperaturas").insertAdjacentHTML("afterbegin", msgHtml);
}

// função de desconexão

function desconectar() {
    console.log("desconectando...");
    document.getElementById("botaoConexao").removeAttribute("class", "btn-danger");
    document.getElementById("botaoConexao").setAttribute("class", "btn btn-success");
    document.getElementById("botaoConexao").textContent = "Conectar!";
    document.getElementById("conexao").setAttribute("class", "invisible p-2 bd-highlight mx-2 my-2 alert alert-success");
    client.disconnect();
}