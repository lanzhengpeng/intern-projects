### 【0464】change view in layout 更改布局中的视图

#### 代码

```cpp
    static void change_view_in_layout(char * view)  
    {  
        UF_CALL(uc6464("", "", view));  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发代码，用于在NX的布局视图中切换当前视图。具体来说，代码实现了以下功能：
>
> 1. static void change_view_in_layout(char * view)：这是一个静态函数，用于切换当前布局视图。
> 2. UF_CALL(uc6464("", "", view))：该行调用了NX的UF函数UF_MODL_change_view_in_layout()，实现了切换视图的功能。其中uc6464是函数的名称，前两个空字符串表示输入参数为空，最后一个参数view是要切换到的视图名称。
> 3. char * view：这是输入参数，表示要切换到的视图名称字符串。
> 4. UF_CALL宏：用于调用NX的UF函数。
> 5. UF_MODL_change_view_in_layout()函数：这是NX提供的UF函数，用于在布局视图中切换当前视图。
> 6. 这段代码封装了NX的UF函数调用，提供了更简洁易用的接口，方便在其他C++代码中调用该函数来切换视图。
>
> 总体来说，这段代码实现了在NX布局视图中根据视图名称切换当前视图的功能，通过封装UF函数调用来简化了调用过程。
>
