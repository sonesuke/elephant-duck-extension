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
#include <string>
#include "pg_regex.hpp"



namespace duckdb {

inline void PgRegexpMatchScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &data = args.data[0];
    auto &re = args.data[1];
    BinaryExecutor::Execute<string_t, string_t, list_entry_t>(data, re, result, args.size(), [&](string_t re, string_t data) {
            vector<std::string> matches;
            int rc = regex_match(data.GetString(),re.GetString(), matches);
            for (auto &match : matches) {
                ListVector::PushBack(result, StringVector::AddString(result, match));
            } 
            return ListVector::GetData(result);
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



static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto elephant_duck_scalar_function = ScalarFunction("elephant_duck", {LogicalType::VARCHAR}, LogicalType::VARCHAR, ElephantDuckScalarFun);
    ExtensionUtil::RegisterFunction(instance, elephant_duck_scalar_function);

    auto pg_regex_matches_scalar_function = ScalarFunction("pg_regexp_match", {LogicalType::VARCHAR, LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, PgRegexpMatchScalarFun);
    ExtensionUtil::RegisterFunction(instance, pg_regex_matches_scalar_function);
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
