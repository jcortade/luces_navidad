# Luces de navidad

Luces de navidad controladas por Arduino

Programa para controlar 100 LEDS (Parámetro variable) usando la libería "FastLED".

Los LEDS utilizan el driver WS2811, son direccionables uno a uno. 

Tiene varios modos que se pueden ir seleccionando (modo manual) con un pulsador, o pueden ir cambiando cada cierto tiempo (modo auto). En uno de los modos se puede variar el color de todos los LEDS con un potenciómetro. Los pulsadores sirven para cambiar auto a manual y apagado, y para cambiar de modo. El LED verde indica si está en manual (intermitente) o automático (fijo). El LED azul indica si se puede usar el potenciómetro para cambiar de color.

[Esquema](https://github.com/jcortade/luces_navidad/blob/main/LucesNavidad_bb.png)
