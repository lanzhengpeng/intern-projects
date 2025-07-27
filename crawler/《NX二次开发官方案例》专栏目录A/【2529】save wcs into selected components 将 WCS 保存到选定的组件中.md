### 【2529】save wcs into selected components 将 WCS 保存到选定的组件中

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void report_csys_info(tag_t csys)  
    {  
        tag_t  
            matrix;  
        double  
            origin[3],  
            vectors[9];  
        if (UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin))) return;  
        WRITE3F(origin);  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, vectors));  
        WRITE9F(vectors);  
    }  
    static tag_t create_duplicate_alive_normal_csys(tag_t csys)  
    {  
        tag_t  
            matrix,  
            new_csys;  
        double  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_create_csys(origin, matrix, &new_csys));  
        return new_csys;  
    }  
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        tag_t  
            instance;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if ((instance = UF_ASSEM_ask_inst_of_part_occ(component)) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_part_name_of_child(instance, partname));  
        }  
        else  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
                instance_name, origin, csys_matrix, transform));  
        }  
    }  
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp,  
            new_csys,  
            new_csys_occ,  
            orig,  
            part = UF_PART_ask_display_part(),  
            wcs;  
        char  
            comp_part[MAX_FSPEC_SIZE+1],  
            disp_part[MAX_FSPEC_SIZE+1];  
        ask_part_shortname(part, disp_part);  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        ECHO("WCS in %s:\n", disp_part);  
        report_csys_info(wcs);  
        orig = UF_ASSEM_ask_work_occurrence();  
        while ((comp = select_a_component("Save WCS into")) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_set_work_occurrence(comp));  
            new_csys = create_duplicate_alive_normal_csys(wcs);  
            if (orig == NULL_TAG)  
                UF_CALL(UF_ASSEM_set_work_part(part));  
            else  
                UF_CALL(UF_ASSEM_set_work_occurrence(orig));  
            UF_CALL(UF_OBJ_set_name(new_csys, "new_csys"));  
            ask_component_shortname(comp, comp_part);  
            ECHO("\nnew_csys in %s:\n", comp_part);  
            report_csys_info(new_csys);  
            new_csys_occ = UF_ASSEM_find_occurrence(comp, new_csys);  
            UF_CALL(UF_OBJ_set_name(new_csys_occ, "new_csys_occ"));  
            ECHO("\nnew_csys_occ in %s:\n", disp_part);  
            report_csys_info(new_csys_occ);  
        }  
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

> 这段NX二次开发代码的主要功能是复制工作坐标系(WCS)到选定的组件中，并报告相关坐标系信息。以下是代码的主要功能：
>
> 1. 初始化和清理：代码首先初始化NX环境，然后调用do_it()函数执行主要功能，最后清理NX环境。
> 2. 获取当前工作部件和工作坐标系：通过UF_PART_ask_display_part()和UF_CSYS_ask_wcs()获取当前工作部件和工作坐标系的标签。
> 3. 报告WCS信息：调用report_csys_info()函数，报告当前工作坐标系的信息。
> 4. 选择组件并复制WCS：通过循环调用select_a_component()选择组件，然后复制WCS到该组件中，并报告新坐标系的详细信息。
> 5. 报告新坐标系在当前工作部件中的信息：调用report_csys_info()函数，报告新坐标系在当前工作部件中的详细信息。
> 6. 支持函数：提供了一些支持函数，如ECHO()用于输出信息，WRITE3F()和WRITE9F()用于输出数组，report_error()用于报告错误，以及一些用于获取部件名称和组件名称的函数。
> 7. 错误处理和日志输出：通过宏定义UF_CALL和report_error()实现错误处理和日志输出。
> 8. 选择组件的过滤：mask_for_components()用于在对话框中过滤只显示组件。
> 9. 坐标系复制：create_duplicate_alive_normal_csys()用于复制坐标系。
> 10. 坐标系信息报告：report_csys_info()用于报告坐标系的信息。
>
> 总的来说，这段代码实现了在NX中复制工作坐标系到选定组件，并报告坐标系信息的功能，通过一系列支持函数实现了选择组件、获取部件名称、报告错误等功能。
>
