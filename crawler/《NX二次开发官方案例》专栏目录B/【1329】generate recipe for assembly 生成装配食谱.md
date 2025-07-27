### 【1329】generate recipe for assembly 生成装配食谱

#### 代码

```cpp
    /*HEAD GENERATE_RECIPE_FOR_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog是V18版本中新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void ask_component_origin_and_matrix(tag_t component, double origin[3],  
        double csys_matrix[9])  
    {  
        double  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static logical ask_list_item_index(uf_list_p_t object_list, tag_t item,  
        int *index)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) break;  
        if (temp == NULL) return FALSE;  
        *index = ii;  
        return TRUE;  
    }  
    static void report_assembly_recipe(tag_t part, uf_list_p_t done)  
    {  
        int  
            ii,  
            index,  
            n_children;  
        tag_t  
            *children,  
            comp_part,  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        double  
            origin[3],  
            matrix[9];  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        uf_list_p_t  
            to_do,  
            temp;  
        if (root_part_occ == NULL_TAG) return;  
        UF_CALL(UF_MODL_create_list(&to_do));  
        ask_part_shortname(part, part_name);  
        WRITE("In ");  
        WRITE(part_name);  
        WRITE(":\n");  
        n_children = UF_ASSEM_ask_part_occ_children(root_part_occ, &children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(children[ii]);  
            ask_part_shortname(comp_part, part_name);  
            WRITE(part_name);  
            WRITE(" at ");  
            ask_component_origin_and_matrix(children[ii], origin, matrix);  
            WRITE3F(origin);  
            WRITE9F(matrix);  
            UF_CALL(UF_MODL_put_list_item(to_do, comp_part));  
        }  
        if (n_children > 0) UF_free(children);  
        WRITE("\n");  
        for (temp = to_do; temp != NULL; temp = temp->next)  
            if (!ask_list_item_index(done, temp->eid, &index))  
            {  
                UF_CALL(UF_MODL_put_list_item(done, temp->eid));  
                report_assembly_recipe(temp->eid, done);  
            }  
        UF_CALL(UF_MODL_delete_list(&to_do));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            done;  
        UF_CALL(UF_MODL_create_list(&done));  
        report_assembly_recipe(part, done);  
        UF_CALL(UF_MODL_delete_list(&done));  
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

> 这段NX二次开发代码的主要功能是输出当前装配体中所有组件的层级关系和位置信息。
>
> 代码的关键功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数执行过程中的错误。
> 2. 定义了一个获取部件短名称的函数ask_part_shortname。
> 3. 定义了一个获取组件原点和变换矩阵的函数ask_component_origin_and_matrix。
> 4. 定义了一个输出数组到日志窗口的函数write_double_array_to_listing_window。
> 5. 定义了一个在列表中查找项目索引的函数ask_list_item_index。
> 6. 定义了一个递归输出装配体组件层级和位置信息的函数report_assembly_recipe。
> 7. 定义了一个递归遍历装配体组件的函数do_it。
> 8. 定义了UF函数ufusr，其中初始化NX，调用do_it函数，然后终止NX。
> 9. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码实现了递归遍历装配体组件，并输出每个组件的层级和位置信息到日志窗口。这有助于理解复杂的装配结构。
>
