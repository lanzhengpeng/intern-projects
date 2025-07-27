### 【0936】create tabular note of specified size 创建指定大小的表格注释

#### 代码

```cpp
    /*HEAD CREATE_TABULAR_NOTE_OF_SPECIFIED_SIZE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_tabnot.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static logical prompt_for_two_integers(char *prompt, char *item1, char *item2,  
        int *number1, int *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        int  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1607(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n_old_columns,  
            n_old_rows,  
            n_new_columns = 5,  
            n_new_rows = 5;  
        tag_t  
            first,  
            new_one,  
            old_one,  
            tabular_note;  
        double  
            height,  
            origin[3],  
            width;  
        UF_TABNOT_cell_prefs_t  
            cell_prefs,  
            old_cell_prefs;  
        UF_TABNOT_section_prefs_t  
            section_prefs;  
    /*  Make sure rows are not set to auto size in order  
        to allow control of their height  
    */  
        UF_CALL(UF_TABNOT_ask_default_cell_prefs(&old_cell_prefs));  
        UF_CALL(UF_TABNOT_ask_default_cell_prefs(&cell_prefs));  
        for (ii = 0; ii < cell_prefs.nm_fit_methods; ii++)  
        {  
            if (cell_prefs.fit_methods[ii] ==  
                UF_TABNOT_fit_method_auto_size_row)  
            {  
                for (jj = ii; jj < cell_prefs.nm_fit_methods; jj++)  
                    cell_prefs.fit_methods[jj] = cell_prefs.fit_methods[jj+1];  
                cell_prefs.nm_fit_methods = cell_prefs.nm_fit_methods - 1;  
                UF_CALL(UF_TABNOT_set_default_cell_prefs(&cell_prefs));  
                break;  
            }  
        }  
        UF_CALL(UF_TABNOT_ask_default_section_prefs(&section_prefs));  
        section_prefs.header_location = UF_TABNOT_header_location_none;  
        section_prefs.attach_point = UF_TABNOT_attach_point_top_left;  
        while (prompt_for_two_integers("Create Table", "Rows", "Columns",  
            &n_new_rows, &n_new_columns) &&  
            specify_position("Table Upper Left Corner", origin))  
        {  
            set_undo_mark("Create Tabular Note");  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            UF_CALL(UF_TABNOT_create(&section_prefs, origin, &tabular_note));  
            UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, 0, &first));  
            UF_CALL(UF_TABNOT_ask_row_height(first, &height));  
            UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, 0, &first));  
            UF_CALL(UF_TABNOT_ask_column_width(first, &width));  
            prompt_for_two_numbers("Specify Sizes", "Row Height", "Column Width",  
                &height, &width);  
            UF_CALL(UF_TABNOT_ask_nm_rows(tabular_note, &n_old_rows));  
            UF_CALL(UF_TABNOT_ask_nm_columns(tabular_note, &n_old_columns));  
            UF_CALL(UF_TABNOT_enable_automatic_update(FALSE));  
        /*  The original rows and columns created using UF_TABNOT_create  
            are not the desired height and width, so delete them.  
        */  
            for (ii = 0; ii < n_old_rows; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, 0, &old_one));  
                UF_CALL(UF_TABNOT_remove_row(old_one));  
                UF_CALL(UF_OBJ_delete_object(old_one));  
            }  
            for (ii = 0; ii < n_old_columns; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, 0, &old_one));  
                UF_CALL(UF_TABNOT_remove_column(old_one));  
                UF_CALL(UF_OBJ_delete_object(old_one));  
            }  
        /*  Create new colums and rows of the desired size 里海译:创建所需大小的列和行 */  
            for (ii = 0; ii < n_new_columns; ii++)  
            {  
                UF_CALL(UF_TABNOT_create_column(width, &new_one));  
                UF_CALL(UF_TABNOT_add_column(tabular_note, new_one,  
                    UF_TABNOT_APPEND));  
            }  
            for (ii = 0; ii < n_new_rows; ii++)  
            {  
                UF_CALL(UF_TABNOT_create_row(height, &new_one));  
                UF_CALL(UF_TABNOT_add_row(tabular_note, new_one,  
                    UF_TABNOT_APPEND));  
            }  
            UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            UF_CALL(UF_TABNOT_update(tabular_note));  
        }  
        UF_CALL(UF_TABNOT_set_default_cell_prefs(&old_cell_prefs));  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，其主要功能是创建一个指定大小的表格注释。代码主要包括以下几个关键部分：
>
> 1. 包含头文件：包含了必要的NX二次开发头文件，如uf.h、uf_ui.h、uf_tabnot.h等。
> 2. 错误报告函数：定义了一个用于报告错误信息的函数report_error，它会在函数调用失败时打印错误信息。
> 3. 提示输入函数：定义了几个用于提示用户输入数字的函数，如prompt_for_two_integers、specify_position等。
> 4. 设置撤销标记函数：定义了一个用于设置撤销标记的函数set_undo_mark。
> 5. 创建表格注释的函数：定义了do_it函数，用于实现创建表格注释的主要逻辑。这个函数会提示用户输入表格的行数和列数，然后根据用户输入在指定位置创建一个表格注释。在创建后，它会删除默认生成的行和列，并重新添加指定大小的行和列。
> 6. 主函数：定义了ufusr函数，用于初始化NX环境，调用do_it函数创建表格注释，并终止NX环境。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载UF时立即卸载。
>
> 总的来说，这段代码通过NX的表格注释API实现了在NX中创建指定大小表格注释的功能，并提供了良好的用户交互。
>
