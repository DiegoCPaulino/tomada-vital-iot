# Tomada Vital — IoT | Global Solution FIAP 2026/2

**Disciplina:** Disruptive Architectures: IoT, IoB & Generative IA  
**Tema da Global Solution:** CitizenTech

## Simulação do circuito no Tinkercad

**[Abrir circuito compartilhado da Tomada Vital no Tinkercad](https://www.tinkercad.com/things/g958jGWu1qs-fantabulous-bruticus-kup/editel?returnTo=%2Fthings%2Fg958jGWu1qs-fantabulous-bruticus-kup&sharecode=KehezaO_vzByj0qiHezNVwroZzdfyHgXVT1UZCgLfPk)**

O circuito contém a montagem Arduino UNO com os componentes e o código usados nos testes da Global Solution. Para reproduzir os cenários, utilize o interruptor deslizante, o potenciômetro e o Monitor Serial, seguindo as instruções deste README e os [registros dos testes](evidencias/README.md).

> Link de compartilhamento fornecido pela equipe. Confirme que ele abre em uma janela anônima antes da entrega, pois links de colaboração podem ter restrições ou expirar.

## Proposta

A Tomada Vital é uma proposta de monitoramento de quedas de energia em equipamentos médicos que dependem de alimentação elétrica. O protótipo demonstra, com um **Arduino UNO simulado no Tinkercad**, como identificar uma queda, estimar a autonomia de uma bateria e sinalizar a prioridade do evento.

O cenário didático usa o dispositivo **TV-001**, um concentrador de oxigênio associado à paciente fictícia Maria Aparecida. A autonomia de 480 minutos e os limites de prioridade são parâmetros **simulados para fins acadêmicos**, não recomendações clínicas.

## Equipe

| Integrante | RM |
|---|---|
| Diego Paulino | 566841 |
| Guilherme Dabul | 559901 |
| Renan Lima | 568321 |
| Ryan Mazali | 567168 |
| Thiago Alexandre | 567486 |

## Protótipo Arduino

Arquivo de código: [`tomada_vital.ino`](tomada_vital.ino).

| Componente | Pino | Função |
|---|---|---|
| Interruptor deslizante | D2 | Simula presença de energia (5V) ou queda (GND) |
| Potenciômetro | A0 | Simula bateria entre 0% e 100% |
| LED verde | D9 | Energia normal |
| LED amarelo | D10 | Queda com prioridade ALTA ou MODERADA |
| LED vermelho | D11 | Queda com prioridade CRITICA |
| Buzzer piezoelétrico | D8 | Alerta sonoro na prioridade CRITICA |

**Montagem:** Arduino UNO R3, protoboard, três LEDs (cada um em série com um resistor de 220 Ω), chave de três terminais, potenciômetro e buzzer. Conectar os negativos dos LEDs e do buzzer ao GND comum. No interruptor, conectar um terminal externo a 5V, o central a D2 e o outro externo ao GND. No potenciômetro, os extremos vão a 5V e GND e o terminal central a A0. O Arduino distribui 5V e GND pelos barramentos da protoboard.

## Funcionamento

Quando a energia está presente, o LED verde fica aceso. Na queda, a bateria simulada é lida pela entrada A0 e o Arduino calcula:

```text
minutos_restantes = max(0, 480 * percentual_bateria / 100 - minutos_desde_a_queda)
```

Para acelerar os testes, **1 segundo da simulação representa 1 minuto**.

| Autonomia restante | Prioridade | Indicação |
|---|---|---|
| Até 120 min | CRITICA | LED vermelho e buzzer |
| De 121 a 360 min | ALTA | LED amarelo |
| Acima de 360 min | MODERADA | LED amarelo |

Os nomes de prioridade não usam acentos no código porque seguem o padrão de integração combinado entre as disciplinas.

## Como executar

1. Abrir [Tinkercad Circuits](https://www.tinkercad.com/circuits).
2. Criar ou abrir um circuito com Arduino UNO e os componentes descritos acima.
3. Conectar os componentes seguindo a tabela de pinos.
4. Abrir **Código → Texto** e substituir o código de exemplo pelo conteúdo completo de [`tomada_vital.ino`](tomada_vital.ino).
5. Iniciar a simulação e abrir o **Monitor Serial** a **9600 baud**.
6. Alternar o interruptor e o potenciômetro para testar os cenários abaixo.

## Testes funcionais

| Cenário | Ação | Comportamento esperado |
|---|---|---|
| 1. Energia normal | Chave em 5V | LED verde; mensagem inicial |
| 2. Queda com 100% | Chave em GND, bateria em 100% | LED amarelo; MODERADA |
| 3. Queda com ~60% | Restabelecer, ajustar a bateria e simular nova queda | LED amarelo; ALTA |
| 4. Queda com ~20% | Restabelecer, ajustar a bateria e simular nova queda | LED vermelho e buzzer; CRITICA |
| 5. Restabelecimento | Voltar a chave para 5V | LED verde; buzzer para; `TV-001;RESTABELECIDO` |

**Observações dos testes realizados no Tinkercad (16/11/2026):** foi observado o LED verde durante energia normal, o LED amarelo ao mudar a chave, mensagens de prioridade `ALTA`, `MODERADA` e `CRITICA` no Monitor Serial, acionamento audível do buzzer na prioridade crítica e emissão de `TV-001;RESTABELECIDO` com retorno ao LED verde. Os resultados são de simulação, não de testes com equipamentos médicos reais. As capturas originais dos testes estão publicadas em [Evidências da simulação](evidencias/README.md).

Trechos observados no Monitor Serial:

```text
TV-001;QUEDA;bateria=35;restante=116;prioridade=CRITICA
TV-001;QUEDA;bateria=34;restante=103;prioridade=CRITICA
TV-001;RESTABELECIDO
```

O valor dos minutos restantes depende da bateria selecionada e do tempo decorrido desde a queda. Para reiniciar o tempo da ocorrência, volte a chave ao estado de energia normal antes de simular outra queda.


## Diagrama visual de arquitetura

O diagrama separa o protótipo efetivamente testado no Tinkercad da comunicação celular/MQTT proposta pela equipe. **[Abrir diagrama de arquitetura e rede](docs/arquitetura.md)**.

## Arquitetura de comunicação proposta

**Importante:** a comunicação em rede descrita nesta seção é **uma proposta de integração**, não uma função já implementada no Arduino do Tinkercad.

```text
Sensores simulados (chave + potenciômetro)
                  |
             Arduino UNO
                  |
      Saída Serial (implementada)
                  |
    Módulo celular (proposto para uso real)
                  |
          Rede celular / Internet
                  |
             Broker MQTT
     tomadavital/TV-001/eventos
                  |
           Serviço integrador
                  |
              API Java
        POST /api/ocorrencias
                  |
        Banco de dados / App
```

A mensagem enviada pela Serial representa o evento que um módulo de comunicação poderia publicar no broker MQTT. Na arquitetura proposta pela equipe, esse evento seria convertido em uma chamada HTTP:

```http
POST /api/ocorrencias
Content-Type: application/json

{ "codigoDispositivo": "TV-001", "bateriaPercentual": 20 }
```

A API Java da solução maior é responsável por registrar a ocorrência e integrar a classificação ao fluxo de atendimento. O exemplo de integração está documentado pela equipe; sua execução ponta a ponta não foi comprovada nesta entrega de IoT.

## Decisões técnicas

- **Arduino UNO:** plataforma usada nas atividades da disciplina e adequada à demonstração de entradas e saídas digitais e analógicas.
- **Chave deslizante:** representa estados estáveis de rede elétrica presente ou ausente.
- **Potenciômetro:** permite variar a bateria manualmente e testar os diferentes limiares.
- **LEDs e buzzer:** proporcionam retorno visual e sonoro ao estado monitorado.
- **Monitor Serial:** torna os eventos verificáveis no Tinkercad e representa a origem dos dados para a integração proposta.
- **MQTT e rede celular (propostos):** permitem conceber o transporte de mensagens do dispositivo até os serviços de software sem depender da rede Wi-Fi local do paciente.

## Limitações e cuidados

- Não foram implementados sensores reais de tensão de rede ou de nível de bateria; chave e potenciômetro são substitutos didáticos.
- O Tinkercad usado não realiza a comunicação celular/MQTT proposta; a saída disponível no protótipo é a Serial.
- O cálculo de autonomia é simplificado e usa parâmetros acadêmicos. Não deve ser empregado para determinar a segurança de pacientes ou de equipamentos de suporte à vida.
- Não foram medidos disponibilidade de rede, latência MQTT, consumo real de energia ou confiabilidade de alarmes em campo.
- O circuito e os testes estão documentados nas [evidências](evidencias/README.md); o [diagrama de arquitetura](docs/arquitetura.md) distingue o protótipo executado da rede apenas proposta.

## Relação com CitizenTech

A proposta busca apoiar cidadãos que dependem de equipamentos médicos elétricos, usando monitoramento e informação de prioridade para fundamentar um fluxo de atendimento. A ideia se relaciona principalmente aos **ODS 3 (Saúde e Bem-Estar)** e **ODS 9 (Indústria, Inovação e Infraestrutura)**. O impacto operacional ainda depende de validações em condições reais.

## Uso de IA generativa

Ferramentas de IA generativa foram utilizadas como apoio à organização documental e à explicação das conexões e dos testes. A equipe deve revisar as informações e demonstrar domínio da implementação durante a apresentação.
