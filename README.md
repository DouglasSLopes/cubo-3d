# Cubo 3D — Wireframe com OpenGL

Trabalho prático de **Computação Gráfica** que implementa um cubo 3D em wireframe utilizando OpenGL/FreeGLUT, com operações manuais de **translação**, **rotação** e **escala** — sem uso de `glTranslate`, `glRotate` ou `glScale`.

---

## Sobre o Projeto

O programa exibe um cubo 3D com projeção perspectiva manual, onde todas as transformações geométricas são calculadas matematicamente no código, respeitando sempre o centro do objeto como pivô.

**Funcionalidades implementadas:**

- Descrição direta dos 8 vértices e 12 arestas do cubo
- Projeção perspectiva manual
- Translação nos eixos X, Y e Z
- Rotação nos eixos X, Y e Z
- Escala uniforme
- Controles por teclado e mouse

---

## Pré-requisitos

### 1. Visual Studio 2022 ou superior

Download gratuito (Community Edition):

> https://visualstudio.microsoft.com/pt-br/downloads/

Durante a instalação, selecione o workload:

- **Desenvolvimento para desktop com C++**

---

### 2. FreeGLUT (versão MSVC)

Biblioteca para criação de janelas OpenGL no Windows.

Download:

> https://www.transmissionzero.co.uk/software/freeglut-devel/

Baixe o pacote **freeglut 3.x.x MSVC Package**.

---

## Configuração do Projeto no Visual Studio

### Passo 1 — Extrair o FreeGLUT

Extraia o conteúdo baixado para `C:\freeglut\`

A estrutura deve ficar assim:

```
C:\freeglut\
├── include\
│   └── GL\
│       ├── freeglut.h
│       ├── freeglut_ext.h
│       ├── freeglut_std.h
│       └── glut.h
├── lib\
│   └── x64\
│       └── freeglut.lib
└── bin\
    └── x64\
        └── freeglut.dll
```

---

### Passo 2 — Criar o Projeto no Visual Studio

1. Abra o Visual Studio
2. Clique em **Criar um novo projeto**
3. Selecione **Aplicativo de Console C++**
4. Dê um nome ao projeto e clique em **Criar**

---

### Passo 3 — Configurar as Propriedades do Projeto

Clique com o botão direito no projeto no **Gerenciador de Soluções** → **Propriedades**

> Certifique-se de que a plataforma está como **x64** no topo da janela de propriedades.

**C/C++ → Geral → Diretórios de Inclusão Adicionais:**

```
C:\freeglut\include
```

**Vinculador → Geral → Diretórios de Biblioteca Adicionais:**

```
C:\freeglut\lib\x64
```

**Vinculador → Entrada → Dependências Adicionais** (adicionar ao início da lista):

```
freeglut.lib
opengl32.lib
glu32.lib
```

Clique em **OK** para salvar.

---

### Passo 4 — Adicionar o código fonte

1. No **Gerenciador de Soluções**, abra o arquivo `.cpp` gerado pelo projeto
2. Apague todo o conteúdo e cole o código do arquivo `cubo3d.cpp`

---

### Passo 5 — Copiar a DLL

Copie o arquivo `freeglut.dll` de:

```
C:\freeglut\bin\x64\freeglut.dll
```

Para a pasta de saída do projeto:

```
...\SeuProjeto\x64\Debug\freeglut.dll
```

---

### Passo 6 — Compilar e Executar

- **Compilar:** `Ctrl + Shift + B`
- **Executar:** `F5`

---

## Controles

| Tecla                   | Ação                  |
| ----------------------- | --------------------- |
| `↑ ↓ ← →`               | Mover nos eixos X e Y |
| `Page Up / Page Down`   | Mover no eixo Z       |
| `W / S`                 | Rotacionar no eixo X  |
| `A / D`                 | Rotacionar no eixo Y  |
| `Q / E`                 | Rotacionar no eixo Z  |
| `+`                     | Aumentar escala       |
| `-`                     | Diminuir escala       |
| Botão esquerdo do mouse | Aumentar escala       |
| Botão direito do mouse  | Diminuir escala       |
| `ESC`                   | Fechar o programa     |

---

## Solução de Problemas

**Erro: `não é possível abrir o arquivo 'freeglut.lib'`**
→ Verifique se o caminho em **Vinculador → Diretórios de Biblioteca** está correto e aponta para a pasta `x64`.

**Erro: `freeglut.dll não foi encontrado`**
→ Copie o `freeglut.dll` para a mesma pasta do `.exe` gerado (`x64\Debug\`).

**Antivírus bloqueando o `.exe`**
→ Adicione a pasta `x64\Debug\` às exclusões do seu antivírus. O executável gerado pelo Visual Studio pode ser detectado como falso positivo.

**Erro: `não é possível abrir o arquivo fonte "GL/freeglut.h"`**
→ Verifique se o caminho em **C/C++ → Diretórios de Inclusão** está correto e aponta para `C:\freeglut\include`.

---

## Detalhes Técnicos

- **Linguagem:** C++11
- **Biblioteca gráfica:** OpenGL + FreeGLUT
- **Projeção:** Perspectiva manual (sem uso de gluPerspective)
- **Transformações:** Calculadas manualmente via matrizes trigonométricas
- **Renderização:** `GL_LINES` (wireframe)
- **Sem uso de:** `glTranslate`, `glRotate`, `glScale`

---

## Estrutura de Arquivos

```
projeto/
├── cubo3d.cpp        ← Código fonte principal
├── freeglut.dll      ← DLL necessária para execução
└── README.md         ← Este arquivo
```
