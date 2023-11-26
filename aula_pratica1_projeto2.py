#O programa pedir para o usuário informar um peso de referência e uma
#altura de referência. A seguir, o programa deve ler o peso e a altura de 3 pessoas
#diferentes. O programa deve informar a média do peso e da altura das 3 pessoas e a
#quantidade de pessoas que devem ser monitoradas. As pessoas que devem ser
#monitoradas são aquelas que possuem simultaneamente peso acima do peso de
#referência e altura abaixo da altura de referência.

# o usuário informa o peso e a altura de referência
peso_ref = float(input("Informe o peso de referência: "))
alt_ref = float(input("Informe a alura de referência: "))

# O usuário informe os dados das 3 pessoas
peso1 = float(input("Qual o peso(kg) da primeira pessoa? "))
alt1 = float(input("Qual a altura(m) da primeira pessoa? "))
peso2 = float(input("Qual o peso(kg) da segunda pessoa? "))
alt2 = float(input("Qual a altura(m) da segunda pessoa? "))
peso3 = float(input("Qual o peso(kg) da terceira pessoa? "))
alt3 = float(input("Qual a altura(m) da terceira pessoa? "))

# testa quantas pessoas devem ser monitoradas
cont = 0
if peso1>peso_ref and alt1<alt_ref:
    cont = cont + 1
if peso2>peso_ref and alt2<alt_ref:
    cont = cont + 1
if peso3>peso_ref and alt3 < alt_ref :
    cont = cont + 1

# Faz a média de altura e do peso das pessoas 
media_pesos = (peso1 + peso2 + peso3 ) / 3
media_alt = (alt1 + alt2 + alt3) / 3

# Escreve os resultados
res_pessoas_monitoradas = f"A quantidade de pessoas monitoradas = {cont}"
res_media_pesos = f"A média do peso das pessoas = {media_pesos:.2f} kg"
res_media_alt = f"A média da altura das pessoas = {media_alt:.2f} m"
print(res_pessoas_monitoradas)
print(res_media_pesos)
print(res_media_alt)