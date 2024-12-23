

#include <cstdio>

#include <common/FLStringFunction.h>
#include <dump/FLDumpRunner.h>
#include <ui/FLCmdOption.h>

#include <FLIncludeAllLibrary.h>


const TCHAR*	PID_OPTION				= _T( "-p" );
const TCHAR*	PNAME_OPTION			= _T( "-pn" );
const TCHAR*	LEVEL_OPTION			= _T( "-l" );
const TCHAR*	DEST_FOLDER_OPTION		= _T( "-d" );
const TCHAR*	VALUE_TAG				= _T( "=" );

const TCHAR*	LEVEL_SUB_LIGHT_OPTION	= _T( "light" );
const TCHAR*	LEVEL_SUB_MEDIUM_OPTION	= _T( "medium" );
const TCHAR*	LEVEL_SUB_HEAVY_OPTION	= _T( "heavy" );

void	Usage()
{
	::_tprintf( _T( "usage: program_name options\n" ) );
	::_tprintf( _T( "  %s%s[process id]\n" ), PID_OPTION, VALUE_TAG );
	::_tprintf( _T( "  %s%s[process name]\n" ), PNAME_OPTION, VALUE_TAG );
	::_tprintf( _T( "  %s%s[%s, %s, %s]\n" ), LEVEL_OPTION, VALUE_TAG, LEVEL_SUB_LIGHT_OPTION, LEVEL_SUB_MEDIUM_OPTION, LEVEL_SUB_HEAVY_OPTION );
	::_tprintf( _T( "  %s%s[dest folder]\n" ), DEST_FOLDER_OPTION, VALUE_TAG );
}


bool	ParseOption( std::vector< tstring >& vecArgs, const tstring& strDelimiters, FLCmdOption* pOption )
{
	std::vector< tstring >::const_iterator pos = vecArgs.begin();
	std::vector< tstring >::const_iterator end = vecArgs.end();

	for( ; pos != end; ++pos )
	{
		const tstring& strArg = *pos;

		std::vector< tstring > vecOptions;
		FLTokenize( strArg, vecOptions, strDelimiters );

		tstring strName;
		tstring strValue;
		if( vecOptions.size() >= 2 )
		{
			strName = vecOptions[0];
			strValue = vecOptions[1];
		}
		else
		{
			strName = strArg;
		}

		if( !strName.empty() )
		{
			if( pOption->AddValue( FLTrim( strName ), FLTrim( strValue ) ) == false )
			{
				return false;
			}
		}
	}

	return true;
}

bool	ParseOption( int argc, TCHAR* const argv[], const tstring& strDelimiters, FLCmdOption* pOption )
{
	if( argc < 2 || argv == NULL || pOption == NULL )
	{
		return false;
	}

	std::vector< tstring > vecArgs;
	for( int i = 1; i < argc; ++i )
	{
		vecArgs.push_back( argv[i] );
	}

	return ParseOption( vecArgs, strDelimiters, pOption );
}


struct	T_DUMP_PROCESS_VALUE
{
	DWORD	dwProcessID;
	TCHAR	szProcessName[MAX_PATH];
};

bool	GetProcessOption( const FLCmdOption* pOption, T_DUMP_PROCESS_VALUE* pValue )
{
	pValue->dwProcessID = 0;
	pValue->szProcessName[0] = _T( '\0' );

	if( pOption->IsSupplied( PID_OPTION ) )
	{
		tstring strPID = pOption->GetFirstValue( PID_OPTION );
		TCHAR* pStop = NULL;
		pValue->dwProcessID = ::_tcstoul( strPID.c_str(), &pStop, 10 );
	}
	else if( pOption->IsSupplied( PNAME_OPTION ) )
	{
		tstring strPName = pOption->GetFirstValue( PNAME_OPTION );
		FLStrcpy( pValue->szProcessName, _countof( pValue->szProcessName ), strPName.c_str() );
	}

	if( pValue->dwProcessID == 0 && FLStrlen( pValue->szProcessName, _countof( pValue->szProcessName ) ) <= 0 )
	{
		return false;
	}

	return true;
}

int		GetDumpLevel( const FLCmdOption* pOption, int nDefault )
{
	if( pOption->IsSupplied( LEVEL_OPTION ) )
	{
		tstring strLevel = pOption->GetFirstValue( LEVEL_OPTION );
		if( strLevel == LEVEL_SUB_LIGHT_OPTION )
		{
			return FLDUMP_LEVEL_LIGHT;
		}
		else if( strLevel == LEVEL_SUB_MEDIUM_OPTION )
		{
			return FLDUMP_LEVEL_MEDIUM;
		}
		else if( strLevel == LEVEL_SUB_HEAVY_OPTION )
		{
			return FLDUMP_LEVEL_HEAVY;
		}
	}

	return nDefault;
}

void	GetDestFolder( const FLCmdOption* pOption, TCHAR* szDestFolder, size_t cchDestFolder )
{
	if( pOption->IsSupplied( DEST_FOLDER_OPTION ) )
	{
		tstring strDest = pOption->GetFirstValue( DEST_FOLDER_OPTION );
		FLStrcpy( szDestFolder, cchDestFolder, strDest.c_str() );
	}
	else
	{
		FLGetProcessFolder( ::GetCurrentProcess(), szDestFolder, cchDestFolder );
	}
}

int		_tmain( int argc, TCHAR* argv[] )
{
	FLCmdOption kCmdOption;
	kCmdOption.AddOption( PID_OPTION );
	kCmdOption.AddOption( PNAME_OPTION );
	kCmdOption.AddOption( LEVEL_OPTION );
	kCmdOption.AddOption( DEST_FOLDER_OPTION );

	if( ParseOption( argc, argv, VALUE_TAG, &kCmdOption ) == false )
	{
		Usage();
		return 0;
	}

	T_DUMP_PROCESS_VALUE tValue;
	::memset( &tValue, 0, sizeof( tValue ) );
	int nDumpLevel = FLDUMP_LEVEL_HEAVY;
	TCHAR szDestFolder[MAX_PATH] = { 0, };

	if( GetProcessOption( &kCmdOption, &tValue ) == false )
	{
		Usage();
		return 0;
	}

	nDumpLevel = GetDumpLevel( &kCmdOption, nDumpLevel );
	GetDestFolder( &kCmdOption, szDestFolder, _countof( szDestFolder ) );

	FLGenericDumper::GetInstance()->SetDumpFolder( szDestFolder );
	if( tValue.dwProcessID != 0 )
	{
		FLDumpRunner::DumpProcess( FLGenericDumper::GetInstance(), nDumpLevel, tValue.dwProcessID );
	}
	else
	{
		FLDumpRunner::DumpProcess( FLGenericDumper::GetInstance(), nDumpLevel, tValue.szProcessName );
	}

	return 0;
}
