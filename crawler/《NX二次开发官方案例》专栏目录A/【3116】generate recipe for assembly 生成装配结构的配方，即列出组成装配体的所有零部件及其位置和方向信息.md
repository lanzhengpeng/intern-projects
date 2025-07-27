### 【3116】generate recipe for assembly 生成装配结构的配方，即列出组成装配体的所有零部件及其位置和方向信息

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解的是：

UF_print_syslog 是一个新功能，在 V18 版本中首次引入。它的作用是打印系统日志。这个函数在 V18 版本中新增。 */  
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

> 这段代码是一个NX二次开发程序，主要功能是生成当前显示部件的装配食谱，并将结果输出到列表窗口中。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义函数ask_part_shortname，用于获取部件的简称。
> 3. 定义函数ask_component_origin_and_matrix，用于获取部件的坐标和变换矩阵。
> 4. 定义宏WRITE，用于将文本输出到列表窗口。
> 5. 定义函数write_double_array_to_listing_window，用于将数组输出到列表窗口。
> 6. 定义函数ask_list_item_index，用于在列表中查找元素的索引。
> 7. 定义函数report_assembly_recipe，递归生成装配食谱。
> 8. 定义函数do_it，初始化并调用report_assembly_recipe。
> 9. 定义ufusr函数，作为程序入口，初始化NX，调用do_it，然后终止NX。
> 10. 定义ufusr_ask_unload函数，用于卸载程序。
>
> 整个程序通过递归调用report_assembly_recipe函数，遍历装配树，输出每个部件的简称、位置和变换矩阵，生成整个装配的食谱。
>
