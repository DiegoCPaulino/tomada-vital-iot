# Diagrama de arquitetura e comunicação — Tomada Vital

O diagrama distingue o **protótipo testado no Tinkercad** da **rede proposta pela equipe**, ainda não implementada no simulador.

```mermaid
flowchart TB
    subgraph P["01 · PROTÓTIPO TESTADO NO TINKERCAD"]
      IN["ENTRADAS<br/>Chave D2 · Potenciômetro A0"]
      UNO["ARDUINO UNO · TV-001<br/>Leitura · Autonomia · Prioridade"]
      LED["ALERTAS LOCAIS<br/>LEDs D9/D10/D11 · Buzzer D8"]
      SERIAL["MONITOR SERIAL · 9600 baud<br/>Eventos TV-001"]
      IN -->|sinais| UNO
      UNO -->|alertas| LED
      UNO -->|eventos| SERIAL
    end
    subgraph N["02 · REDE PROPOSTA — NÃO IMPLEMENTADA NO TINKERCAD"]
      CELL["MÓDULO CELULAR<br/>Conectividade proposta"]
      NET["REDE CELULAR<br/>Internet"]
      MQTT["BROKER MQTT<br/>Recebe eventos"]
      BRIDGE["INTEGRAÇÃO<br/>MQTT para HTTP"]
      API["API JAVA<br/>POST /api/ocorrencias"]
      APP["APLICATIVO<br/>Ocorrências e prioridades"]
      CELL --> NET
      NET -->|MQTT| MQTT
      MQTT --> BRIDGE
      BRIDGE -->|HTTP POST| API
      API --> APP
    end
    SERIAL -.->|envio futuro| CELL
    classDef atual fill:#d9f0e9,stroke:#238879,color:#17324d;
    classDef planejado fill:#fff3dc,stroke:#d29c43,color:#664619,stroke-dasharray:5 4;
    class IN,UNO,LED,SERIAL atual;
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
