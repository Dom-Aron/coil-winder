# Coil Winder (Enrolador de Bonina)

> Fork/clonagem do repositório original OpenActuator/CoilWinder.

Propósito

Este repositório contém o código e recursos para um enrolador de bonina (coil winder). A intenção é desenvolver melhorias e adaptar o projeto para usos locais. O desenvolvimento ativo será feito na branch `dev`.

Remotos

- `upstream`: https://github.com/OpenActuator/CoilWinder.git (origem)
- `origin`: https://github.com/Dom-Aron/coil-winder.git (seu fork/clone)

Como começar

1. Clonar o repositório:

```bash
git clone https://github.com/Dom-Aron/coil-winder.git
git remote add upstream https://github.com/OpenActuator/CoilWinder.git
```

2. Trocar para a branch de desenvolvimento:

```bash
git checkout dev
```

3. Sincronizar com o upstream regularmente:

```bash
git fetch upstream
git merge upstream/master
```

Estrutura

O projeto original foi preservado. Faça suas alterações na branch `dev` e abra pull requests para `master` quando estiver pronto.

Contribuição

Use a branch `dev` para desenvolvimento. Mantemos um fluxo simples: desenvolver em `dev`, testar e depois mesclar em `master`.

Licença

Ver a licença no repositório original ou adicionar uma nova conforme desejado.
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
