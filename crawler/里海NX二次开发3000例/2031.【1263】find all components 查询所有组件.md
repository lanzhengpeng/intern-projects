### 【1263】find all components 查询所有组件

#### 代码

```cpp
    static void find_all_components(void)  
    {  
        char  
            color[UF_DISP_MAX_NAME_SIZE+1],  
            part_fspec[MAX_FSPEC_SIZE+1];  
        int  
            i,  
            n;  
        tag_t  
            comp = NULL,  
            part;  
        n = UF_PART_ask_num_parts();  
        for (i=0; i<n; i++)  
        {  
            part = UF_PART_ask_nth_part(i);  
            UF_PART_ask_part_name(part, part_fspec);  
            printf("Part tag %d - %s\n", part, part_fspec);  
            do  
            {  
                UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp));  
                if (comp)  
                {  
                    ask_obj_color(comp, color);  
                    printf("  component tag %d is %s\n", comp, color);  
                }  
            } while (comp);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发代码，其主要功能是遍历当前NX部件中的所有组件，并打印出每个组件的名称和颜色。具体步骤如下：
>
> 1. 首先，使用UF_PART_ask_num_parts()函数获取当前部件中的组件数量n。
> 2. 然后，通过循环遍历每个组件，使用UF_PART_ask_nth_part()函数获取组件的tag，并使用UF_PART_ask_part_name()函数获取组件的名称，并打印出来。
> 3. 接下来，使用do-while循环遍历每个组件中的子组件，使用UF_OBJ_cycle_objs_in_part()函数获取子组件的tag。
> 4. 如果获取到子组件，则调用ask_obj_color()函数获取子组件的颜色，并打印出子组件的tag和颜色。
> 5. 重复上述步骤，直到所有组件和子组件都被遍历。
> 6. 最后，该函数可以找到当前部件中的所有组件，并打印出每个组件的名称和颜色信息。
>
> 整体来看，这段代码通过遍历NX部件的组件树，实现了查找并打印组件名称和颜色信息的功能，对于了解NX模型结构非常有帮助。
>
