### 【1702】open comp as from grip 打开组件作为从抓取

#### 代码

```cpp
    /*HEAD OPEN_COMP_AS_FROM_GRIP CCC UFUN */  
    /************************************************************  
    entity/ comp, new_comp  
    string/ comp_name(132)  
    grargs/ comp, new_comp, comp_name  
    $$  The value of new_comp will be the entity  
    $$  identifier of the component that was added in  
    $$  the ufunc program.  
    mask/ 63  
    ident/'Select component', comp, resp  
        jump/ bck:, cncl:, ,resp  
    comp_name = 'component3.prt' $$ entire filespec preferred  
            xspawn/ ufun,'/path/to/open_comp_as_from_grip.sl'  
    bck:  
    cncl:  
    HALT  
    ***************************************************************/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        tag_t   comp, new_comp, part;  
        char    part_name[MAX_FSPEC_SIZE+1], refset_name[MAX_ENTITY_NAME_SIZE+1],  
                instance_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        char    new_comp_name[132];  
        double  origin[3], csys_matrix[9], transform[4][4];  
        UF_args_t   args[3];  
        UF_PART_load_status_t   load_status;  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &comp;  
        args[1].type = UF_TYPE_TAG_T;  
        args[1].length = 0;  
        args[1].address = &new_comp;  
        args[2].type = UF_TYPE_CHAR;  
        args[2].length = 0;  
        args[2].address = &new_comp_name;  
        UF_CALL(UF_ask_grip_args( 3, args));  
        UF_CALL( UF_ASSEM_ask_component_data( comp, part_name, refset_name,  
                              instance_name, origin, csys_matrix, transform));  
        uc1601( part_name, 1);  
        part = UF_ASSEM_ask_prototype_of_occ( comp);  
        UF_CALL( UF_PART_close( part, 1, 1));  
        UF_CALL( UF_PART_open_component_as( comp, part_name, new_comp_name,  
                                            &new_comp, &load_status));  
    }  
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

> 这段NX Open C++代码用于在NX装配环境下，将装配组件以新文件的形式打开。
>
> 主要功能包括：
>
> 1. 获取用户选择的组件，获取组件的名称、原型、位置等信息。
> 2. 关闭组件的原型文件。
> 3. 以新文件的形式打开组件，并获取新文件的标识。
>
> 代码的主要逻辑如下：
>
> 1. 使用UF_ask_grip_args()获取用户选择的组件标识、新组件标识和组件名称。
> 2. 使用UF_ASSEM_ask_component_data()获取组件的名称、引用集名称、实例名称、原点、坐标系矩阵和变换矩阵等信息。
> 3. 使用UF_ASSEM_ask_prototype_of_occ()获取组件的原型标识。
> 4. 使用UF_PART_close()关闭组件的原型文件。
> 5. 使用UF_PART_open_component_as()以新文件的形式打开组件，并获取新文件的标识。
>
> 在代码中，通过定义宏UF_CALL来简化错误报告，并在出现错误时调用report_error()函数打印错误信息。主函数ufusr()用于初始化NX Open，调用do_it()执行主要功能，然后终止NX Open。
>
> 总的来说，这段代码实现了在NX装配环境中，以新文件的形式打开用户选择的组件的功能。
>
