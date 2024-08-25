#define DUCKDB_EXTENSION_MAIN

#include "elephant_duck_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>


#include <locale>
#include <codecvt>
#include "pg_regex.hpp"

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>


namespace duckdb {

inline void PgRegexMatchesScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &re = args.data[0];
    auto &data = args.data[1];
    BinaryExecutor::Execute<string_t, string_t, int32_t>(
	    re, data, result, args.size(),
	    [&](string_t re, string_t data) {
            vector<std::wstring> matches;
            int nmatch = 0;
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wre = converter.from_bytes(re.GetString());
            std::wstring wdata = converter.from_bytes(data.GetString());

            int rc = regex_matches(wre, wdata, nmatch, matches);
            if (rc != 0) {
                
            }
            return matches.size();
        });
}

inline void ElephantDuckScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "ElephantDuck "+name.GetString()+" 🐥");;
        });
}

inline void ElephantDuckOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "ElephantDuck " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto elephant_duck_scalar_function = ScalarFunction("elephant_duck", {LogicalType::VARCHAR}, LogicalType::VARCHAR, ElephantDuckScalarFun);
    ExtensionUtil::RegisterFunction(instance, elephant_duck_scalar_function);

    auto pg_regex_matches_scalar_function = ScalarFunction("pg_regex_matches", {LogicalType::VARCHAR, LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, PgRegexMatchesScalarFun);
    ExtensionUtil::RegisterFunction(instance, pg_regex_matches_scalar_function);

    // Register another scalar function
    auto elephant_duck_openssl_version_scalar_function = ScalarFunction("elephant_duck_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, ElephantDuckOpenSSLVersionScalarFun);
    ExtensionUtil::RegisterFunction(instance, elephant_duck_openssl_version_scalar_function);
}

void ElephantDuckExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string ElephantDuckExtension::Name() {
	return "elephant_duck";
}

std::string ElephantDuckExtension::Version() const {
#ifdef EXT_VERSION_ELEPHANT_DUCK
	return EXT_VERSION_ELEPHANT_DUCK;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void elephant_duck_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::ElephantDuckExtension>();
}

DUCKDB_EXTENSION_API const char *elephant_duck_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
