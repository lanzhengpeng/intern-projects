### 【2157】report individual and accumulated mass and volume 报告单个和累计的质量和体积

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** This starts at the displayed part, and reports the Volume and Mass  
    ** of each individual body found in the assembly, and then reports  
    ** the accumulated total mass and volume.  To check a subassembly,  
    ** make the subassembly the displayed part.  
    **  
    ** First tested in NX8.  
    **  
    *****************************************************************************/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
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
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            units = 0;  
        tag_t   
            part = NULL_TAG,  
            solid = NULL_TAG;  
        double  
            acc[11] = { 0.99, 0,0,0,0,0,0,0,0,0,0 },  
            mass_props[47],  
            stats[13],  
            thisVolume = 0.,  
            thisMass = 0.,  
            accumMass = 0.,   
            accumVolume = 0.;  
        part = UF_PART_ask_display_part();  
        if(NULL_TAG == part) return;  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_METRIC)  
            units = UF_MODL_grams_centimeters;  
        else  
            units = UF_MODL_pounds_inches;  
        solid = ask_next_solid_body(part, solid);  
        while (solid != NULL_TAG)  
        {  
            tag_t  
                solids[1];  
            if(UF_ASSEM_is_occurrence(solid))  
            {  
                solids[0] = UF_ASSEM_ask_prototype_of_occ(solid);  
            }  
            else  
            {  
                solids[0] = solid;  
            }  
            UF_CALL(UF_MODL_ask_mass_props_3d(solids, 1, 1, units, 1., 1, acc,  
                                                            mass_props, stats));  
            thisMass = mass_props[2];  
            accumMass += thisMass;  
            thisVolume = mass_props[1];  
            accumVolume += thisVolume;  
            ECHO("Volume of body %d:  %f\n", solid, thisVolume);  
            ECHO("  Mass of body %d:  %f\n\n", solid, thisMass);  
            solid = ask_next_solid_body(part, solid);  
        }   
        ECHO("Accumulated Volume: %f\n\n", accumVolume);  
        ECHO("Accumulated   Mass: %f\n", accumMass);  
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

> 这段代码是用于NX的二次开发，主要功能是遍历显示的部件中的实体，计算每个实体的体积和质量，并累加得到总体积和总质量。
>
> 代码的主要功能如下：
>
> 1. 定义了一个ECHO宏，用于输出信息到日志窗口和系统日志。
> 2. 定义了一个report_error函数，用于处理UF函数的返回码，如果出错则输出错误信息。
> 3. 定义了一个ask_next_solid_body函数，用于遍历部件中的实体，并返回下一个实体。
> 4. do_it函数是主函数，首先获取当前显示的部件，然后遍历实体，计算每个实体的体积和质量，并累加得到总体积和总质量。
> 5. ufusr函数是NX调用的主入口函数，初始化后调用do_it函数，最后终止。
> 6. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 该代码主要用于快速获取当前显示部件中所有实体的体积和质量统计信息，对NX的二次开发具有实用价值。
>
