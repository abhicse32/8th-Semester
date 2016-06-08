; ModuleID = 'test3.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @foo(i32** %p) #0 {
entry:
  %p.addr = alloca i32**, align 8
  store i32** %p, i32*** %p.addr, align 8
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32**, align 8
  %j = alloca i32*, align 8
  %m = alloca i32*, align 8
  store i32 0, i32* %retval
  %call = call noalias i8* @malloc(i64 40) #2
  %0 = bitcast i8* %call to i32**
  store i32** %0, i32*** %i, align 8
  %call1 = call noalias i8* @malloc(i64 20) #2
  %1 = bitcast i8* %call1 to i32*
  store i32* %1, i32** %j, align 8
  %call2 = call noalias i8* @malloc(i64 20) #2
  %2 = bitcast i8* %call2 to i32*
  store i32* %2, i32** %m, align 8
  %3 = load i32** %j, align 8
  %4 = load i32*** %i, align 8
  store i32* %3, i32** %4, align 8
  %5 = load i32*** %i, align 8
  call void @foo(i32** %5)
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
