# Desenvolver um programa que processa as vendas de um mercado
# Se o total for maior que 50 reais aplicar desconto de 2%
# primeira parte mostra a tabela para o usuário
linha0 = "Código do produto\t Produto\t Valor Unitário" 
linha1 = "1\t\t\t Pão\t\t R$1,50"
linha2 = "2\t\t\t Leite(1L)\t R$4,65"
linha3 = "3\t\t\t Biscoito(300g)  R$6,93"
print(linha0)
print(linha1)
print(linha2)
print(linha3)

#definindo constantes
porcentagem_desconto = 0.98
preco_pao = 1.5
preco_leite = 4.65
preco_biscoito = 6.93
# pergunta p/ o usuário qual o produto e a quantidade do produto
cod = int(input("Qual o código do produto desejado ? "))
quantidade = int(input("Qual a quantidade desejada? "))

# cálcula o valor total levando em conta o desconto

if cod == 1 :
    total = preco_pao*quantidade

elif cod == 2 :
    total = preco_leite*quantidade

elif cod == 3 :
    total = preco_biscoito*quantidade

else :
    print("Código inválido")
    total = 0
if total > 50.00 :
    total = total*porcentagem_desconto

# imprime o resultado
linha_final = f"O total a ser pago = {total:.2f}"
print(linha_final)