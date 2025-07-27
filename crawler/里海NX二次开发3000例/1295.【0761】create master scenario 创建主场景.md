### 【0761】create master scenario 创建主场景

#### 代码

```cpp
    /*HEAD CREATE_MASTER_SCENARIO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中的信息，UF_print_syslog是在V18版本中新增的功能，并且要求只回答与翻译相关的译文，不要添加其他无关的内容。因此，对于这一句的翻译，我们可以直接翻译为：

UF_print_syslog是在V18版本中新增的功能。 */  
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
    static int prompt_block_parms(char *sides[3])  
    {  
        char  
            menu[3][16] = { "height", "width", "length" };  
        double  
            values[3];  
        int  
            junk,  
            i,  
            irc;  
        for (i=0; i<3; i++) sscanf(sides[i], "%lf", &values[i]);  
        irc = uc1609("Enter block parameters", menu, 3, values, &junk);  
        if (irc == 3 || irc == 4)  
        {  
            for (i=0; i<3; i++) sprintf(sides[i], "%f", values[i]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void create_blocks(void)  
    {  
        char  
            *sides[3];  
        double  
            corner[3];  
        int  
            i,  
            sel[2] = { 0, 0 };  
        tag_t  
            block;  
        for (i=0; i<3; i++)  
            UF_CALL(allocate_memory(20*sizeof(char), (void**)&sides[i]));  
        for (i=0; i<3; i++) sprintf(sides[i], "%f", 1.0);  
        while ((prompt_block_parms(sides)) &&  
              ((uc1616("Select corner point", sel, 0, corner)) == 5))  
            UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner, sides, &block));  
        for (i=0; i<3; i++) UF_free(sides[i]);  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* UF_SF_is_scenario_part returns 0 (FALSE) when the part IS a scenario part 译:当零件是场景的一部分时，UF_SF_is_scenario_part返回0（FALSE）。 */  
    static logical is_scenario_part(void)  
    {  
        if (!UF_SF_is_scenario_part()) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            scenario_name[26]={"scenario_1"};  
        tag_t  
            new_scenario_tag;  
        create_blocks();  
        UF_CALL(UF_PART_save());  
        WRITE_L(is_scenario_part());  
        UF_CALL(UF_SF_create_scenario (scenario_name, &new_scenario_tag ));  
        WRITE_L(is_scenario_part());  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于创建一个场景(scenario)。下面是代码的主要功能介绍：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了提示用户输入块参数的函数prompt_block_parms。
> 4. 定义了分配内存的函数allocate_memory。
> 5. 定义了创建块的函数create_blocks，其中会提示用户输入块的长宽高参数，并选择块的角点位置，然后调用UF_MODL_create_block1接口创建块。
> 6. 定义了向列表窗口写入逻辑值的函数write_logical_to_listing_window。
> 7. 定义了判断当前是否为场景零件的函数is_scenario_part。
> 8. 定义了执行主要逻辑的函数do_it，包括创建块并保存，判断是否为场景零件，创建新场景等。
> 9. 定义了ufusr主函数，用于初始化和执行do_it函数，在结束时进行终止。
> 10. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码通过交互方式让用户输入参数，创建了场景中的块，并保存了场景。
>
