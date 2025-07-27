### 【2496】report symb thread data 报告螺纹符号数据

#### 代码

```cpp
    static void report_symb_thread_data(UF_MODL_symb_thread_data_t *thread)  
    {  
        int  
            sub,  
            type;  
        UF_CALL(UF_OBJ_ask_type_and_subtype(thread->cyl_face, &type, &sub));  
        printf("Cylindrical face tag = %d, type %d, subtype %d\n",   
            thread->cyl_face, type, sub);  
        UF_CALL(UF_OBJ_ask_type_and_subtype(thread->start_face, &type, &sub));  
        printf("Start face tag = %d, type %d, subtype %d\n",   
            thread->start_face, type, sub);  
        printf("Thread axis direction = (%f,%f,%f)\n",  
            thread->axis_direction[0], thread->axis_direction[1],  
            thread->axis_direction[2]);  
        printf("Number of starts = %d\n", thread->num_starts);  
        if (thread->include_instances == UF_MODL_INCL_INSTANCES)  
            printf("Include instances = Yes\n");  
        else  
            printf("Include instances = No\n");  
        if (thread->rotation == UF_MODL_RIGHT_HAND)  
            printf("Rotation = Right_hand\n");  
        else  
            printf("Rotation = Left_hand\n");  
        if (thread->length_flag == UF_MODL_FIXED_LENGTH)  
            printf("Thread length flag = Fixed length\n");  
        else   
            printf("Thread length flag = Full thread\n");  
        if (thread->tapered == UF_MODL_TAPERED)   
            printf("Thread tapered = Yes\n");  
        else   
            printf("Thread tapered = No\n");  
        printf("Length expression %s\n", thread->length);  
        printf("Form = %s\n", thread->form);  
        printf("Callout = %s\n", thread->callout);  
        printf("Method = %s\n", thread->method);  
        printf("Major diameter expression %s\n", thread->major_dia);  
        printf("Minor diameter expression %s\n", thread->minor_dia);  
        printf("Tapped diameter expression %s\n", thread->tapped_dia);  
        printf("Pitch expression %s\n", thread->pitch);  
        printf("Angle expression %s\n", thread->angle);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，用于报告螺纹特征的相关信息。
>
> 主要功能包括：
>
> 1. 查询并输出螺纹特征的起始面、圆柱面、轴线方向等基本属性信息。
> 2. 输出螺纹特征的其他参数，如螺距、螺纹长度、螺纹形式、螺纹头数等。
> 3. 判断并输出螺纹特征是否包含实例、螺纹旋转方向、螺纹长度类型等属性。
> 4. 输出螺纹直径、螺距等参数的表达式。
> 5. 使用UF_OBJ_ask_type_and_subtype函数查询螺纹特征对象的类型和子类型，并输出。
> 6. 使用printf函数格式化输出螺纹特征信息。
>
> 通过这段代码，开发者可以快速获取和报告螺纹特征的相关信息，方便调试和验证NX二次开发功能。
>
