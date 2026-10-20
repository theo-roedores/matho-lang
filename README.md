# matho-lang

Um avaliador de expressões matemáticas escrito em C, feito do zero e sem bibliotecas além da libc.

O projeto foi construído em **um dia** com um objetivo de aprendizado: **entender como as linguagens de programação funcionam por dentro**. Uma calculadora de expressões é o menor exemplo completo do mesmo pipeline que linguagens reais usam: ler texto, transformá-lo em tokens, organizá-lo em uma árvore e executá-la.

```
$ ./matho "(2 + 3) * 4 ^ 2"
Expression: '(2 + 3) * 4 ^ 2'
Result: '80.000000'
```

## Funcionalidades

| Operação | Símbolo | Precedência |
|---|---|---|
| Potência | `^` | alta (3) |
| Multiplicação / Divisão | `*` `/` | média (2) |
| Soma / Subtração | `+` `-` | baixa (1) |
| Agrupamento | `( )` | sobrescreve a precedência |

- Números inteiros e decimais (`3.14`)
- Parênteses aninhados
- Precedência de operadores respeitada (`2 + 2 * 4` = `10`)
- Associatividade à esquerda (`10 - 3 - 2` = `5`)
- Mensagens de erro para expressões inválidas (`2 + 2 *`, `(2 + 3`, `2 + 3)`)

## Como compilar e usar

```bash
./build.sh
```

Passe a expressão como argumento. Com aspas ou sem:

```bash
./matho "1 + 2 + 3"
./matho 1 + 2 + 3
./matho "(1^99 + (2 - 58) / (5*5*2 / 3^3)) + (5 * 2 - 99 + 41)^2"
```

Sem argumentos, o programa mostra a ajuda e roda uma expressão de exemplo (resultado ≈ `2274.76`).

> O `*` pode ser expandido pelo shell. Use aspas ao digitar `./matho "2 * 3"`.

## Licença

MIT License

Copyright (c) 2026 Théo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.