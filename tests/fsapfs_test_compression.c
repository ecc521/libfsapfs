/*
 * Library compression type test program
 *
 * Copyright (C) 2018-2026, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsapfs_test_libcerror.h"
#include "fsapfs_test_libfsapfs.h"
#include "fsapfs_test_macros.h"
#include "fsapfs_test_memory.h"
#include "fsapfs_test_unused.h"

#include "../libfsapfs/libfsapfs_compression.h"
#include "../libfsapfs/libfsapfs_definitions.h"

uint8_t fsapfs_test_compression_deflate_compressed_data1[ 24 ] = {
	0x78, 0x9c, 0x63, 0x60, 0x64, 0x62, 0x66, 0x61, 0x65, 0x63, 0xe7, 0xe0, 0xe4, 0xe2, 0xe6, 0xe1,
	0xe5, 0xe3, 0x07, 0x00, 0x02, 0xb8, 0x00, 0x79 };

uint8_t fsapfs_test_compression_deflate_uncompressed_data1[ 17 ] = {
	0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

uint8_t fsapfs_test_compression_lzvn_compressed_data1[ 19 ] = {
	0xe0, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
	0x0e, 0x0f, 0x06 };

uint8_t fsapfs_test_compression_lzvn_uncompressed_data1[ 17 ] = {
	0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

uint8_t fsapfs_test_compression_uncompressed_data1[ 16 ] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

/* Type 9/10 stored: 0xCC sentinel + 16 bytes of raw data */
uint8_t fsapfs_test_compression_stored_data1[ 17 ] = {
	0xcc, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

/* Bad sentinel — first byte is not 0xCC */
uint8_t fsapfs_test_compression_stored_bad_sentinel[ 17 ] = {
	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

/* LZFSE-compressed "The quick brown fox jumps over the lazy dog. " x20 (900 bytes plaintext, 84 bytes compressed)
 * Generated on macOS 26.4 via compression_encode_buffer(COMPRESSION_LZFSE=0x801)
 */
uint8_t fsapfs_test_compression_lzfse_compressed_data1[ 84 ] = {
	0x62, 0x76, 0x78, 0x6e, 0x84, 0x03, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x54, 0x68,
	0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b, 0x20, 0x62, 0x72, 0x6f, 0x77, 0x6e, 0x20, 0x66, 0x6f,
	0x78, 0x20, 0x6a, 0x75, 0x6d, 0x70, 0x73, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x74, 0x00, 0x1f,
	0xea, 0x6c, 0x61, 0x7a, 0x79, 0x20, 0x64, 0x6f, 0x67, 0x2e, 0x20, 0x38, 0x2d, 0xf0, 0xff, 0xf0,
	0xff, 0xf0, 0xff, 0xf0, 0x0e, 0xe2, 0x2e, 0x20, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x62, 0x76, 0x78, 0x24 };

/* 0xFF stored-chunk: sentinel byte + 16 bytes of raw data (LZFSE rsrc stored-chunk format) */
uint8_t fsapfs_test_compression_lzfse_stored_chunk[ 17 ] = {
	0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

/* Tests the libfsapfs_decompress_data function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_decompress_data(
     void )
{
	uint8_t uncompressed_data[ 16 ];
	uint8_t lzfse_uncompressed_data[ 1024 ];
	uint8_t lzfse_expected_data[ 900 ];

	libcerror_error_t *error      = NULL;
	size_t lzfse_phrase_index     = 0;
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_compressed_data1,
	          24,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fsapfs_test_compression_uncompressed_data1,
	          16 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fsapfs_test_compression_uncompressed_data1,
	          16 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzvn_compressed_data1,
	          19,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fsapfs_test_compression_uncompressed_data1,
	          16 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fsapfs_test_compression_uncompressed_data1,
	          16 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test STORED (types 9 and 10): 0xCC sentinel + verbatim data
	 */
	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_stored_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_STORED,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fsapfs_test_compression_uncompressed_data1,
	          16 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Bad sentinel — must return error */
	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_stored_bad_sentinel,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_STORED,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test LZFSE (types 11 and 12): real compressed data generated on macOS 26.4
	 */
	uncompressed_data_size = 1024;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzfse_compressed_data1,
	          84,
	          LIBFSAPFS_COMPRESSION_METHOD_LZFSE,
	          lzfse_uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 900 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* The plaintext is "The quick brown fox jumps over the lazy dog. " (45 bytes) repeated 20 times
	 */
	for( lzfse_phrase_index = 0;
	     lzfse_phrase_index < 20;
	     lzfse_phrase_index++ )
	{
		memory_copy(
		 &( lzfse_expected_data[ lzfse_phrase_index * 45 ] ),
		 "The quick brown fox jumps over the lazy dog. ",
		 45 );
	}
	result = memory_compare(
	          lzfse_uncompressed_data,
	          lzfse_expected_data,
	          900 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test LZFSE 0xFF stored-chunk (rsrc fork uncompressed block)
	 */
	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzfse_stored_chunk,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZFSE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSAPFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSAPFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fsapfs_test_compression_uncompressed_data1,
	          16 );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          NULL,
	          24,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

/* TODO check if this test is correct */
	result = libfsapfs_decompress_data(
	          uncompressed_data,
	          16,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_compressed_data1,
	          24,
	          -1,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_compressed_data1,
	          24,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          NULL,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_compressed_data1,
	          24,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          NULL,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_uncompressed_data1,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = (size_t) SSIZE_MAX + 1;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	uncompressed_data_size = 16;

	/* Test libfsapfs_decompress_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzvn_uncompressed_data1,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = (size_t) SSIZE_MAX + 1;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_stored_data1,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSAPFS_COMPRESSION_METHOD_STORED,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_stored_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_STORED,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = (size_t) SSIZE_MAX + 1;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_stored_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_STORED,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	uncompressed_data_size = 16;

	/* Test libfsapfs_decompress_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

#if defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	uncompressed_data_size = 16;

	/* Test libfsapfs_decompress_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_stored_data1,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_STORED,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	uncompressed_data_size = 16;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzfse_stored_chunk,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSAPFS_COMPRESSION_METHOD_LZFSE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzfse_stored_chunk,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZFSE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	uncompressed_data_size = (size_t) SSIZE_MAX + 1;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzfse_stored_chunk,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZFSE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSAPFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSAPFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	uncompressed_data_size = 16;

	/* Test libfsapfs_decompress_data with memcpy failing
	 */
	fsapfs_test_memcpy_attempts_before_fail = 0;

	result = libfsapfs_decompress_data(
	          fsapfs_test_compression_lzfse_stored_chunk,
	          17,
	          LIBFSAPFS_COMPRESSION_METHOD_LZFSE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	if( fsapfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsapfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSAPFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSAPFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FSAPFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSAPFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSAPFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSAPFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSAPFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSAPFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSAPFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

	FSAPFS_TEST_RUN(
	 "libfsapfs_decompress_data",
	 fsapfs_test_decompress_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSAPFS_DLL_IMPORT ) */
}

