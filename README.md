# Dueliel

Você (@) é um arqueiro preso num campo de grama alta.<br/>
Se esgueirando pela grama alta existe algo ou alguém tentando te matar.<br/>
Sua única escolha é retribuir o favor.<br/>                                               
                                                                                                                                                                                                                                                                                                  
Em seu turno, há duas opções [m]over ou [a]tirar.                                                                            
Após escolher sua ação, informe em qual direção será realizada. As possíveis direções são:<br/>

nw nn ne<br/>
ww @  ee<br/>
sw ss se<br/>

Por fim, caso a ação escolhida seja [m]over, resta dizer a distancia desejada (valor entre 1-3).                                                                               
Boa sorte e que vença o melhor.

## Compilar
### Dependencias

Para compilar **dueliel** a biblioteca liblua5.3-dev é necessária<br/>
```
sudo apt install liblua5.3-dev lua5.3
```
Utilizando a makefile, o executável **dueliel** é produzido
```
make
```
## Executar

Existem 2 formas de executar **dueliel**:<br/>
1. Para criar uma partida simplesmente execute **dueliel**
```
./dueliel
```
2. Para se conectar à uma partida existente execute **dueliel** e passe como argumento o IP do host
```
./dueliel 127.0.0.1
```
