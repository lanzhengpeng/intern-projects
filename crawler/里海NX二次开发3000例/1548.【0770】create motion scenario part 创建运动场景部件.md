### 【0770】create motion scenario part 创建运动场景部件

#### 代码

```cpp
    /*HEAD CREATE_MOTION_SCENARIO_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_sf.h>  
    #include <uf_mech.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
        for (i=0; i<3; i++) sides[i] = (char *)calloc(20, sizeof(char));  
        for (i=0; i<3; i++) sprintf(sides[i], "%f", 1.0);  
        while ((prompt_block_parms(sides)) &&  
              ((uc1616("Select corner point", sel, 0, corner)) == 5))  
            UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner, sides, &block));  
    }  
    static void do_it(void)  
    {  
        char  
            part_name[]={"a_blank_part.prt"},  
            scenario_name[26]={"scenario_1"},  
            cp1[30]={"mechanism_name"};  
        tag_t  
            part,  
            new_scenario_tag,  
            nr2;  
        UF_CALL(UF_PART_new(part_name,UF_PART_ENGLISH,&part));  
        create_blocks();  
        UF_CALL(UF_SF_create_scenario (scenario_name, &new_scenario_tag ));  
        UF_CALL(uc6001(cp1, &nr2));  
        UF_CALL(UF_PART_save());   
        UF_CALL(UF_PART_close(new_scenario_tag,0,0));  
        UF_CALL(UF_PART_set_display_part(part));  
        UF_CALL(UF_PART_save());     
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个提示用户输入块参数的函数prompt_block_parms，并使用用户自定义对话框uc1609实现。
> 3. 定义了一个创建块实体的函数create_blocks，并调用UF_MODL_create_block1接口实现。
> 4. 定义了一个主函数do_it，用于创建空白零件、添加块实体、创建运动学仿真场景、保存零件。
> 5. 定义了ufusr函数作为NX二次开发的入口点，在该函数中调用do_it函数实现主要功能。
> 6. 定义了ufusr_ask_unload函数，用于卸载二次开发代码。
>
> 总体而言，这段代码通过创建空白零件，在零件中添加块实体，并保存零件，最后创建一个运动学仿真场景，实现了在NX中创建运动仿真场景的功能。
>
