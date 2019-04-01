#include "settings.h"

int main( void ) {
	
	settings_manager::get( )->floating_point( "rbot_stoopid", -83.09283 );

	settings_manager::get( )->export_config( "test" );
	settings_manager::get( )->load_config( "test" );

	auto setting = settings_manager::get( )->find_setting<settings::c_float>( "rbot_stoopid" );

	printf( std::to_string( setting->value ).c_str( ) );

	Sleep( 50000000 );

	return 0;

}