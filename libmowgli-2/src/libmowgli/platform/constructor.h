/*
 * constructor.h
 * Code for setting up automatic initializer functions portably.
 *
 * Copyright (c) 2012 William Pitcock <nenolod@dereferenced.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice is present in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MOWGLI_PLATFORM_CONSTRUCTOR_H__
#define __MOWGLI_PLATFORM_CONSTRUCTOR_H__

#ifdef _MSC_VER

/*
 * Automatic constructors are not yet officially supported in MSVC, however,
 * there is a similar feature where functions in the ".CRT$XCU" section are
 * evaluated prior to DllMain(), main() and friends.
 *
 * See http://blogs.msdn.com/b/vcblog/archive/2006/10/20/crt-initialization.aspx
 * for more information.
 */
#define MOWGLI_BOOTSTRAP_FUNC(func) \
	static void __cdecl func(void); \
	__declspec(allocate(".CRT$XCU")) void (__cdecl *func##_)(void) = func; \
	static void __cdecl func(void)

#elif defined(__GNUC__) || defined(__SUNPRO_C)

#define MOWGLI_BOOTSTRAP_FUNC(func) \
	static void func(void) __attribute__((constructor)); \
	static void func(void)

#else

#error MOWGLI_BOOTSTRAP_FUNC not implemented for your platform :(

#endif

#endif
