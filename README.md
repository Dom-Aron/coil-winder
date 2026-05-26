# Coil Winder

Fork de trabalho baseado no projeto publico
[OpenActuator/CoilWinder](https://github.com/OpenActuator/CoilWinder).

Este repositorio existe para adaptar o projeto original de enrolador de
bobinas ao nosso uso local: produzir muitas bobinas de forma mais repetivel,
documentada e facil de manter.

## Origem

- Repositorio original: https://github.com/OpenActuator/CoilWinder
- Fork de trabalho: https://github.com/Dom-Aron/coil-winder
- Licenca original: MIT, preservada neste repositorio em `LICENSE`
- Branch de desenvolvimento: `dev`

O projeto original foi mantido como referencia. As mudancas deste fork devem
ficar documentadas aqui para facilitar o uso em laboratorio/oficina e permitir
sincronizacao futura com o upstream quando fizer sentido.

## Objetivo deste fork

Estamos usando este repositorio para transformar o projeto original em uma base
mais adequada para producao repetida de bobinas. As prioridades iniciais sao:

- entender e documentar o firmware Arduino e o software de controle;
- preservar a referencia ao projeto original e sua licenca;
- organizar um fluxo de desenvolvimento no GitHub;
- adaptar parametros, comandos e interface para uso pratico em lote;
- melhorar confiabilidade, calibracao e retomada do processo quando necessario.

## Estrutura do projeto

- `Source/OpenCoilWinder/OpenCoilWinder.ino`: firmware Arduino.
- `Source/Windows/CoilWinder.xlsm`: software de controle original para Windows.
- `Source/Communication_Commands.xlsx`: comandos de comunicacao.
- `Images/`: fotos e diagramas do projeto.
- `Modeling_FreeCAD/`: modelos mecanicos em FreeCAD.

## Fluxo de trabalho

O desenvolvimento ativo acontece na branch `dev`.

Para clonar este fork:

```bash
git clone https://github.com/Dom-Aron/coil-winder.git
cd coil-winder
```

Para manter a referencia ao repositorio original:

```bash
git remote add upstream https://github.com/OpenActuator/CoilWinder.git
git fetch upstream
```

Para sincronizar futuramente com o projeto original:

```bash
git fetch upstream
git merge upstream/master
```

## Referencia original

O texto abaixo foi preservado do README original para manter o contexto do
projeto base.

---

# Coil Winder (Version 1.2)

This is a Small Coil Winder Project.

1. Structures
 - Coil winder : Arduino Nano, Motors
 - Control SW : PC Software (GUI, Communication)
 - Communication : RS232C Serial Communication (suppored arduino)

2. Mechanical Parts
 - All structure parts were made by 3D Printer
 - You can get the structure parts in the "3D Design" directory.
 - The sturcutre parts were designed by FreeCAD (http://www.freecadweb.org).

3. Electronical Parts
 - Arduino Nano
 - Rotating Motor : 17HS2408, L9110 H-bridge
 - Slider Motor : Slider Motor, L9110 H-bridge
 - DC-DC Down Convertor : LM2596

4. Problems
 - Rotating Motor is not enough power to rotate a coil.
 - Guide Stage can't use because of centering and frictional force about a guide rotator.
 - To need making a control software only for this winder.

5. Youtube Video : https://youtu.be/hM2LF0jpD0Y
<br><br>
6. Detail Information
 - English : <a href="https://solenoid.or.kr/direct_eng.php?address=https://solenoid.or.kr/openactuator/open_coil_winder_eng.htm">Open Source Coil Winder Homepage Link (English)</a>
 - Korean  : <a href="https://solenoid.or.kr/direct_kor.php?address=https://solenoid.or.kr/openactuator/open_coil_winder_kor.htm">Open Source Coil Winder Homepage Link (Korean)</a>
<br><br>
<img src="https://www.solenoid.or.kr/openactuator/OpenCoilWinder/CoilWinder_V1.2.jpg" border="0" alt="">
