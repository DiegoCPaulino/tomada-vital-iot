# Diagrama de arquitetura e comunicação — Tomada Vital

Este diagrama representa **exatamente o protótipo Arduino testado** e, separadamente, a **proposta de conectividade da equipe** para a Global Solution 2026/2. A ligação celular/MQTT **não foi implementada no Tinkercad**; a mensagem é exibida no Monitor Serial.

```mermaid
flowchart TB
    subgraph P["01 · PROTÓTIPO TESTADO NO TINKERCAD"]
      IN["ENTRADAS SIMULADAS<br/>Chave deslizante D2<br/>Potenciômetro A0"]
      UNO["ARDUINO UNO · TV-001<br/>Leitura de energia e bateria<br/>Autonomia = 480 × bateria/100<br/>− minutos desde a queda"]
      LED["SAÍDAS LOCAIS<br/>LED verde D9<br/>LED amarelo D10<br/>LED vermelho D11<br/>Buzzer D8"]
      SERIAL["MONITOR SERIAL · 9600 baud<br/>TV-001;QUEDA;...;prioridade=CRITICA"]
      IN -->|sinais| UNO
      UNO -->|sinalização| LED
      UNO -->|eventos| SERIAL
    end
    subgraph N["02 · COMUNICAÇÃO EM REDE PROPOSTA — NÃO IMPLEMENTADA NO TINKERCAD"]
      CELL["Módulo de comunicação celular<br/>(hardware futuro)"]
      NET["Rede celular / Internet"]
      MQTT["Broker MQTT<br/>tomadavital/TV-001/eventos"]
      BRIDGE["Serviço integrador<br/>MQTT → HTTP / JSON"]
      API["API JAVA DA EQUIPE<br/>POST /api/ocorrencias<br/>Registra ocorrência e prioridade"]
      APP["Sistema / aplicativo<br/>Ocorrência e fila de atendimento"]
      CELL -->|dados móveis| NET
      NET -->|MQTT| MQTT
      MQTT -->|evento| BRIDGE
      BRIDGE -->|HTTP POST| API
      API -->|consulta / resposta| APP
    end
    SERIAL -.->|mensagem que seria transmitida| CELL
    classDef atual fill:#d9f0e9,stroke:#238879,color:#17324d;
    classDef planejado fill:#fff3dc,stroke:#d29c43,color:#664619,stroke-dasharray:5 4;
    class UNO,IN,LED,SERIAL atual;
    class CELL,NET,MQTT,BRIDGE,API,APP planejado;
```

## Identificação dos componentes

| Elemento | Implementado? | Papel |
|---|---|---|
| Chave deslizante D2 | Sim (simulado) | Presença/queda de energia |
| Potenciômetro A0 | Sim (simulado) | Percentual de bateria |
| Arduino UNO | Sim (simulado) | Leitura, cálculo e classificação |
| LEDs D9/D10/D11 e buzzer D8 | Sim (simulado) | Alertas locais |
| Monitor Serial (9600 baud) | Sim | Exibe eventos e resultados |
| Módulo celular | Não | Transporte de eventos na implantação proposta |
| Rede celular e Internet | Não | Conectividade externa proposta |
| Broker MQTT | Não | Recebe eventos no tópico previsto |
| Serviço integrador | Não | Converte evento MQTT em requisição HTTP |
| API Java e aplicativo | Não demonstrados nesta entrega IoT | Registro e visualização previstos pela equipe |

## Mensagens e integração previstas pela equipe

Tópico MQTT proposto: `tomadavital/TV-001/eventos`

Exemplo de evento do protótipo:

```text
TV-001;QUEDA;bateria=20;restante=96;prioridade=CRITICA
```

Exemplo do envio ao endpoint da API Java indicado no material da equipe:

```http
POST /api/ocorrencias
Content-Type: application/json

{"codigoDispositivo":"TV-001","bateriaPercentual":20}
```

**Limites e honestidade técnica:** o módulo celular e a comunicação MQTT são parte do desenho proposto, não do circuito executado no simulador; a API Java pertence ao projeto integrado da equipe. A bateria e o sensor de energia foram representados por componentes de simulação, e os limiares de autonomia são exemplos acadêmicos, não critérios médicos validados.
