% (1) Compilar o programa em C
system('gcc Plotagem.c -o plotagem');

% (2) Executar o programa
system('./plotagem');

% (3) Ler os dados dos arquivos gerados
tabela_insercao = readtable('dados_insercao.txt');
tabela_remocao = readtable('dados_remocao.txt');

% Extrair colunas da tabela de insercao
valores_insercao = tabela_insercao{:, 1};
comparacoes_lista_insercao = tabela_insercao{:, 2};
comparacoes_heap_insercao = tabela_insercao{:, 3};

% Extrair colunas da tabela de remocao
valores_remocao = tabela_remocao{:, 1};
comparacoes_lista_remocao = tabela_remocao{:, 2};
comparacoes_heap_remocao = tabela_remocao{:, 3};

% ===============================
% GRÁFICO A: INSERÇÃO
% ===============================

figure('Color', 'w', 'Position', [200, 200, 850, 500]);
% Plotando só os pontos, sem linhas
plot(valores_insercao, comparacoes_lista_insercao, 'o', 'Color', [0 0.4 0.8], ...
    'MarkerSize', 5, 'DisplayName', 'Lista Ordenada');
hold on;
plot(valores_insercao, comparacoes_heap_insercao, 's', 'Color', [0.8 0.1 0.1], ...
    'MarkerSize', 5, 'DisplayName', 'Heap Binário');
hold off;

title('Análise da Inserção', 'FontSize', 14, 'FontWeight', 'bold');
xlabel('Valor Inserido', 'FontSize', 11);
ylabel('Nº de Comparações', 'FontSize', 11);
legend('show', 'Location', 'northwest');
grid on;
set(gca, 'GridAlpha', 0.5, 'FontSize', 11);

% Salvar imagem
exportgraphics(gca, 'grafico_insercao.png', 'Resolution', 300);

% ===============================
% GRÁFICO B: REMOÇÃO
% ===============================

figure('Color', 'w', 'Position', [200, 200, 850, 500]);
% Plotando só os pontos, sem linhas
plot(valores_remocao, comparacoes_lista_remocao, '^', 'Color', [0 0.6 0.3], ...
    'MarkerSize', 5, 'DisplayName', 'Lista Ordenada');
hold on;
plot(valores_remocao, comparacoes_heap_remocao, 'd', 'Color', [0.9 0.5 0], ...
    'MarkerSize', 5, 'DisplayName', 'Heap Binário');
hold off;

title('Análise da Remoção', 'FontSize', 14, 'FontWeight', 'bold');
xlabel('Valor Removido', 'FontSize', 11);
ylabel('Nº de Comparações', 'FontSize', 11);
legend('show', 'Location', 'southwest');
grid on;
set(gca, 'GridAlpha', 0.5, 'FontSize', 11);

% Salvar imagem
exportgraphics(gca, 'grafico_remocao.png', 'Resolution', 300);
