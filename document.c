#include <stdio.h>
#include <string.h>
#include "document.h"

int init_document(Document *doc, const char *name) {

   if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {

        return FAILURE;
    }

    strcpy(doc->name, name);
    doc->number_of_paragraphs = 0;
    return SUCCESS;

}

int reset_document(Document *doc) {

    if (doc == NULL) {

        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    return SUCCESS;

}

int print_document(Document *doc) {

    int i, j;

    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);
        }
        
        if (i < doc->number_of_paragraphs - 1 && doc->paragraphs[i].number_of_lines > 0) {
            printf("\n");
        }
    }

    return SUCCESS;
}


int add_paragraph_after(Document *doc, int paragraph_number) {

    int i;

    if (doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS || paragraph_number > doc->number_of_paragraphs) {

        return FAILURE;
    }

    for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {

        doc->paragraphs[i] = doc->paragraphs[i - 1];

    }

    doc->paragraphs[paragraph_number].number_of_lines = 0;
    doc->number_of_paragraphs++;

    return SUCCESS;
}


int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {

    Paragraph *paragraph;
    int i;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES ||
        line_number > doc->paragraphs[paragraph_number - 1].number_of_lines || new_line == NULL) {
        return FAILURE;
    }

    paragraph = &doc->paragraphs[paragraph_number - 1];

    for (i = paragraph->number_of_lines; i > line_number; i--) {

        strcpy(paragraph->lines[i], paragraph->lines[i - 1]);

    }

    strncpy(paragraph->lines[line_number], new_line, MAX_STR_SIZE);
    paragraph->lines[line_number][MAX_STR_SIZE] = '\0';
    paragraph->number_of_lines++;

    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {

    if (doc == NULL || number_of_lines == NULL || paragraph_number > doc->number_of_paragraphs) {

        return FAILURE;
    }
    
    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    return SUCCESS;

}

int get_number_lines(Document *doc, int *number_of_lines) {
   
   int i, total_lines = 0;

    if (doc == NULL || number_of_lines == NULL) {

        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; i++) {

        total_lines += doc->paragraphs[i].number_of_lines;
    }
    
    *number_of_lines = total_lines;
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {

    Paragraph *paragraph;

   if (doc == NULL || new_line == NULL || paragraph_number > doc->number_of_paragraphs || 
        doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES) {
        return FAILURE;
    }

    paragraph = &doc->paragraphs[paragraph_number - 1];
    strncpy(paragraph->lines[paragraph->number_of_lines], new_line, MAX_STR_SIZE);
    paragraph->lines[paragraph->number_of_lines][MAX_STR_SIZE] = '\0';
    paragraph->number_of_lines++;

    return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number) {

    Paragraph *paragraph;
    int i;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {

        return FAILURE;
    }

    paragraph = &doc->paragraphs[paragraph_number - 1];

    for (i = line_number - 1; i < paragraph->number_of_lines - 1; i++) {

        strcpy(paragraph->lines[i], paragraph->lines[i + 1]);
    }

    paragraph->number_of_lines--;
    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {

    int i, current_paragraph = 0;

    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }

    add_paragraph_after(doc, 0);

    for (i = 0; i < data_lines; i++) {
        if (strcmp(data[i], "") == 0) {
            current_paragraph++;
            add_paragraph_after(doc, current_paragraph);
        } else {
            append_line(doc, current_paragraph + 1, data[i]);
        }
    }

    return SUCCESS;
    
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    
    int i, j;
    char *position;

    if (doc == NULL || target == NULL || replacement == NULL) {

        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; i++) {

        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {

            while ((position = strstr(doc->paragraphs[i].lines[j], target)) != NULL) {

                char buffer[MAX_STR_SIZE + 1];
                strncpy(buffer, doc->paragraphs[i].lines[j], position - doc->paragraphs[i].lines[j]);
                buffer[position - doc->paragraphs[i].lines[j]] = '\0';
                strcat(buffer, replacement);
                strcat(buffer, position + strlen(target));
                strncpy(doc->paragraphs[i].lines[j], buffer, MAX_STR_SIZE);
            }
        }
    }

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
   
   char highlighted[MAX_STR_SIZE + 1];
    snprintf(highlighted, MAX_STR_SIZE + 1, "%s%s%s", HIGHLIGHT_START_STR, target, HIGHLIGHT_END_STR);
    return replace_text(doc, target, highlighted);
}

int remove_text(Document *doc, const char *target) {
  
    return replace_text(doc, target, "");

}
