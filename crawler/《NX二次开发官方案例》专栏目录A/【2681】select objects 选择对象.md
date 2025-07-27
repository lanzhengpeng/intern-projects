### 【2681】select objects 选择对象

#### 代码

```cpp
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中进行对象选择的方法。代码的具体功能如下：
>
> 1. 方法名称：select_objects，用于选择对象。
> 2. 参数：prompt是提示信息，objects是输出参数，用于返回选中的对象数组。
> 3. 函数实现：使用UF_UI模块的UF_UI_select_with_class_dialog函数，显示一个选择对话框，允许用户在工作部件范围内选择对象。
> 4. 处理选择结果：获取用户选择的响应码resp和选择对象的数量cnt，并将选中的对象保存到objects数组中。
> 5. 关闭高亮：遍历objects数组，关闭每个对象的高亮显示。
> 6. 返回值：返回选中的对象数量cnt。
> 7. 异常处理：使用UF_CALL宏处理函数调用，若调用失败则抛出异常。
>
> 总的来说，这段代码实现了在NX CAD系统中让用户通过对话框选择对象的功能，并返回了选中的对象数组及其数量。
>
