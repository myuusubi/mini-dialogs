mini-dialogs
===
A minimal version of Tiny File Dialogs for use within the Oxymora engine

FAQ
---

#### What's the license?

The original Tiny File Dialogs (TinyFD) library is licensed under the zlib
License. Mini Dialogs (MiniDLG) simply combines the header and implementation
files of TinyFD, and slightly modifies them to let the compiler optimize out 
the parts we do not use in MiniDLG. Our contributions are small, and as such:

MiniDLG itself is in the public domain. The python script `minify.py` as well
as all code in the `src` directory can be used for whatever project you want.
We are unsure of why anyone would use MiniDLG, but regardless, you have no 
legal obligation to do anything else.

If your legal team will not accept the public domain license, each file will
also have an explicit dual-license, and you can choose for them to be under
the New BSD License (BSD-3-Clause) instead.

#### Why does MiniDLG exist?

The Oxymora game engine only requires a very small subset of the power TinyFD
provides, so MiniDLG takes advantage of modern optimizing compilers to strip
down the exported symbols to only what we require. Notifications, input boxes,
color pickers, etc., are all handled by the Oxymora GUI. We only need to show
a message box in the event that the engine cannot be initialized, and display
a familiar, platform specific Open File dialog.

#### How do I use MiniDLG?

MiniDLG incorporates TinyFD into a single-header file library, so simply add
an include for the .h file where you would like to use it. In addition, have
_exactly one_ C/C++ source file where you define the `MINI_DLG_IMPLEMENTATION`
macro, which will enable the function definitions and instantiate the code.

We have the flat, single-header file in the Releases page of this repository.
It is recommended that you use this version, as it is very similar to what we 
use internally for Oxymora, but you can always run `python3 minify.py` if you 
would like to generate the header for yourself.