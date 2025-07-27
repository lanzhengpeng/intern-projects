### 【2002】report assembly tree modified status 报告装配树修改状态

#### 代码

```cpp
    /*HEAD REPORT_ASSEMBLY_TREE_MODIFIED_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        logical  
            nxman;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void report_children_modified_status(tag_t comp, int indent)  
    {  
        logical  
            modified,  
            nxman;  
        int  
            ii,  
            loaded,  
            n;  
        tag_t  
            *children,  
            part;  
        char  
            *cp_name,  
            cli_name[MAX_FSPEC_SIZE+1],  
            comp_name[MAX_FSPEC_SIZE+1],  
            space[MAX_LINE_SIZE+1] = { "" };  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        for (ii = 0; ii < indent; ii++) strcat(space, " ");  
        n = UF_ASSEM_ask_part_occ_children(comp, &children);  
        for (ii = 0; ii < n; ii++)  
        {  
            ask_component_shortname(children[ii], comp_name);  
            WRITE(space);  
            WRITE(comp_name);  
            WRITE(" ");  
            if (nxman)  
            {  
                sprintf(cli_name, "@DB/%s", comp_name);  
                UF_CALL(UF_UGMGR_convert_name_from_cli(cli_name, &cp_name));  
                loaded = UF_PART_is_loaded(cp_name);  
                UF_free(cp_name);  
            }  
            else  
                loaded = UF_PART_is_loaded(comp_name);  
            if (!loaded)  
            {  
                WRITE_L(loaded);  
            }  
            else  
            {  
                part = UF_ASSEM_ask_prototype_of_occ(children[ii]);  
                modified = UF_PART_is_modified(part);  
                WRITE_L(modified);  
            }  
            report_children_modified_status(children[ii], indent+1);  
        }  
        if (n > 0) UF_free(children);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        logical  
            modified;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        ask_part_shortname(part, part_name);  
        modified = UF_PART_is_modified(part);  
        WRITE(part_name);  
        WRITE(" ");  
        WRITE_L(modified);  
        report_children_modified_status(UF_ASSEM_ask_root_part_occ(part), 1);  
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

> 这段代码是一个用于报告NX部件树修改状态的二次开发程序。主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了写入清单窗口的宏WRITE和WRITE_L。
> 3. 定义了获取部件和组件简称的函数ask_part_shortname和ask_component_shortname。
> 4. 定义了递归报告子组件修改状态的函数report_children_modified_status。
> 5. 定义了主函数do_it，用于获取当前显示部件的修改状态，并递归报告所有子组件的修改状态。
> 6. 定义了ufusr函数，用于初始化NX并执行do_it，然后终止NX。
> 7. 定义了卸载函数ufusr_ask_unload，用于立即卸载该程序。
>
> 该程序通过递归遍历当前显示部件的组件树，获取每个组件的简称，判断其是否被修改，并将修改状态打印到清单窗口。这有助于检查NX部件树的修改情况。
>
