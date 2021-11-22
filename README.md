```
  ██████  ██░ ██  ▄▄▄       ██▀███   ██ ▄█▀ ██ ▄█▀▓█████  ██▀███  
▒██    ▒ ▓██░ ██▒▒████▄    ▓██ ▒ ██▒ ██▄█▒  ██▄█▒ ▓█   ▀ ▓██ ▒ ██▒
░ ▓██▄   ▒██▀▀██░▒██  ▀█▄  ▓██ ░▄█ ▒▓███▄░ ▓███▄░ ▒███   ▓██ ░▄█ ▒
  ▒   ██▒░▓█ ░██ ░██▄▄▄▄██ ▒██▀▀█▄  ▓██ █▄ ▓██ █▄ ▒▓█  ▄ ▒██▀▀█▄  
▒██████▒▒░▓█▒░██▓ ▓█   ▓██▒░██▓ ▒██▒▒██▒ █▄▒██▒ █▄░▒████▒░██▓ ▒██▒
▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒ ▒▒   ▓▒█░░ ▒▓ ░▒▓░▒ ▒▒ ▓▒▒ ▒▒ ▓▒░░ ▒░ ░░ ▒▓ ░▒▓░
░ ░▒  ░ ░ ▒ ░▒░ ░  ▒   ▒▒ ░  ░▒ ░ ▒░░ ░▒ ▒░░ ░▒ ▒░ ░ ░  ░  ░▒ ░ ▒░
░  ░  ░   ░  ░░ ░  ░   ▒     ░░   ░ ░ ░░ ░ ░ ░░ ░    ░     ░░   ░ 
      ░   ░  ░  ░      ░  ░   ░     ░  ░   ░  ░      ░  ░   ░     
                                                            
                    %                              .,. . *, 
                    %%                 .,(&%*///(*(#*(,*//% 
                   #%%&           ,,,***,*  ....*(*#%%&&&&  
                   #%%%&///((*/****//..../*.,,,,,,,/%%%%%   
                  #(((((((///*//*,...,(%%%%&%&@@@@@&&%%#    
  \           .(##%#%##(((((..,*,*((%&&@@@@@@@@@@@@@&      
   %        *####%%%%%(#####*//((((&@@@@@@@@@@@@@&@@        
   %&     //(/%/%%((&&/&&(/#/(/%*(/.., .,  .(**@,@          
   %%  %(((#%(((&(&(@&#((//((&(%(((&(&/(%((&&%&&&&%%%       
   &%##%%%&&&&&&%&%&&&&@&&%%%&%%%&&&&&&&&&&&&&&&&&@         
   && &&@&&&&&%%%%&%%###%&%%%%&&&&%&&&&&&&&&/               
  %@  (@   &&&&%%%#((#(@@&&&&&&&&&&&&                       
 %            &((((@           &&&&&@                       
           #%&@                 &&&&@                       
        %@                       @&@                                                              
```
#                            SHARKKER SAGA

## SHARKKER 3D
https://matheusr42.github.io/computer-graphics-UFABC/sharkker3d/
Release 2021/11/21

## SHARKKER 2D
https://matheusr42.github.io/computer-graphics-UFABC/sharkker/
Release 2021/10/31

https://github.com/MatheusR42/computer-graphics-UFABC


by  **Matheus Araujo**      *<https://github.com/MatheusR42>*   matheus.victor@aluno.ufabc.edu.br

&&  **Giovanne Galdino**    *<https://linktr.ee/galdino.gio>*   giovanne.galdino@aluno.ufabc.edu.br

```
                             UFABC
                    Univesidade Federal do ABC
                    
                MCTA008-17 ♦ Computação Gráfica 
                Prof. Bruno Augusto Dorta Marques
```
## Game
Sharkker 3D é um jogo tridimensional desenvolvido com base em sua versão 2D [Sharkker](https://matheusr42.github.io/computer-graphics-UFABC/sharkker/) e na aplicação [Starfield](https://hbatagelo.github.io/cg/starfield.html) utilizada na disciplina de Computação Gráfica MCTA008-17 da UFABC. 
Foi utilizado o OpenGL como API gráfica, com programação em C++ e framework ABCg também desenvolvido para acompanhamento da disciplina de CG.

O objetivo do jogo é guiar adiante um tubarão pelo vasto oceano, enquanto grandes fragmentos de corais tentam atingí-lo.
Conforme o animal avança e desvia dos obstáculos, são somados pontos ao canto superior esquerdo da tela.
Caso o tubarão colida com algum coral, ele se machucará e perderá uma de suas três vidas, indicadas no canto superior direito. Ao chegar à zero, o jogo acaba.

![Sharkker3D](public/3D_Sharkker.png)

Conforme o tubarão avança, o nível de dificuldade aumenta, com maior geração de corais e maior velocidade. Porém, essa dificuldade é limitada à um nível justo de jogabilidade.
Não há um final para esse jogo e o objetivo é ir o mais longe possível, somando mais pontos.

## Jogar
É possível jogar Sharkker a partir do primeiro link deste markdown em WebAssembly no GitHub Pages ou fazer um build seguindo a seguinte documentação: https://hbatagelo.github.io/cg/config.html

## Comandos
← Mover para a esquerda

→ Mover para a direita

↑ Mover para cima

↓ Mover para baixo


## Objetos
### Shark
Sharkker acompanha a trajetória de um tubarão tigre *(Galeocerdo cuvier)*, comum em toda a costa brasileira. Esse tipo de esqualo pode chegar até 9 metros de comprimento. O jogo se desenvolve em algum lugar entre o litoral nordestino continental e a ilha de Fernando de Noronha. 
Pouco se sabe sobre a motivação do animal, seu passado e seus objetivos. Não conhecemos a personalidade dele, seu nível de agressidade ou o porquê ele está naquela situação.
A cultura humana nos leva à acreditar que tubarões são animais perigosos e violentos. Porém, vemos seu habitat ser constantemente ameaçado. Então, que escolha eles têm?

O modelo 3D em formato .obj do animal foi gratuitamente baixado em [Free3D](https://free3d.com/3d-model/shark-v5--166344.html).

![Shark - Blender](public/blender.png)

Conforme movimenta-se, é possível enxergar vários ângulos do tubarão.

![Shark - Centered](public/3D_Shark_Centered.png)
![Shark - Left](public/3D_Shark_Left.png) ![Shark - Up](public/3D_Shark_Up.png) ![Shark - Down](public/3D_Shark_Down.png) ![Shark - Up](public/3D_Shark_DiagonalUpperRight.png)

### Corals
Os corais são gerados em uma posição aleatória ao final do eixo de profundidade do espaço do jogo e seguem em direção ao tubarão. Ao passarem por ele, soma-se um ponto para cada coral, eles são reaproveitados e posicionados de forma aleatória nas distâncias iniciais novamente.

O modelo 3D em formato .obj dos corais foi gratuitamente baixado em [3dmdb](https://3dmdb.com/en/3d-model/spike-ball-by-fabanafabana/8963728/).

![Corals](public/3D_Sharkker_SharkkCorals.png)

### Bubbles and Ocean
A cor do oceano, ao fundo de todos os objetos, foi baseada justamente na cor "Ocean Blue" #006994 (de palheta R000 G105 B148). Porém, há um leve escurecimento para melhor contraste com os outros componentes do jogo.
A mecânica das bolhas foi herdada do código de Starfield e, assim como os corais, é gerada aleatóriamente ao final do ângulo de visão e segue em direção ao tubarão. Ao passar por ele, são reaproveitadas e posicionadas de forma aleatória nas distâncias iniciais novamente.

![Bubbles and Ocean](public/3D_Sharkker_Bubbles.png)

## Features
• Sistema de pontuação

• Sistema de vidas

• Display do título do jogo, game over e créditos

• Tubarão realiza movimentos conforme nada e recebe comandos do jogador.

• Corais com movimento rotacional e que progridem em direção ao player de posições aleatórios

• Além de ficar vermelho, o tubarão recebe invencibilidade momentânea ao perder cada uma das duas primeiras vidas 

![Shark - Hurt](public/3D_Shark_Hurt.png)

----
## ABCg
Development framework accompanying the course MCTA008-17 Computer Graphics at [UFABC](https://www.ufabc.edu.br/).

### Credits
Developed by Harlen Batagelo.

### License

ABCg is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
