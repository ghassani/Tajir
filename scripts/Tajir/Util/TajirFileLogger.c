/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirFileLogger
{
	protected FileHandle 	m_handle;
	protected string 		m_filename;

	private static const string ID_ERROR 	= "E";
	private static const string ID_NOTICE 	= "N";
	private static const string ID_WARNING 	= "W";
	private static const string ID_INFO 	= "I";
	private static const string ID_DEBUG 	= "D";

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  filename  The filename
	 */
	void TajirFileLogger( string filename )
	{
		m_filename 	= filename;

		m_handle = OpenFile( filename, FileMode.APPEND );

		if ( m_handle == 0 )
		{
			Error( string.Format( "Error Opening %1", filename ));
		}
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirFileLogger()
	{
		if ( m_handle != 0 ) {
			CloseFile( m_handle );
		}
	}

	/**
	 * @brief      Gets the file handle.
	 *
	 * @return     The file handle.
	 */
	FileHandle GetFileHandle()
	{
		return m_handle;
	}

	/**
	 * @brief      Gets the file name.
	 *
	 * @return     The file name.
	 */
	string GetFileName()
	{
		return m_filename;
	}

	/**
	 * @brief      Writes a line.
	 *
	 * @param[in]  id       The identifier
	 * @param[in]  message  The message
	 * @param[in]  tag      The tag
	 */
	void WriteLine( string id, string message, string tag = "" )
	{
		if ( id != "" && tag != "" )
		{
			FPrintln( m_handle, string.Format( "[%1] [%2] %3", id, tag, message ) );
		}
		else if ( id != "" )
		{
			FPrintln( m_handle, string.Format( "[%1] %2", id, message ) );
		}
		else if ( tag != "" )
		{
			FPrintln( m_handle, string.Format( "[%1] %2", tag, message ) );
		} 
		else
		{
			FPrintln( m_handle, message );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  message  The message
	 * @param[in]  tag      The tag
	 */
	void Error( string message, string tag = "" )
	{
		WriteLine( ID_ERROR, message, tag );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  message  The message
	 * @param[in]  tag      The tag
	 */
	void Notice( string message, string tag = "" )
	{
		WriteLine( ID_NOTICE, message, tag );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  message  The message
	 * @param[in]  tag      The tag
	 */
	void Warning( string message, string tag = "" )
	{
		WriteLine( ID_WARNING, message, tag );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  message  The message
	 * @param[in]  tag      The tag
	 */
	void Info( string message, string tag = "" )
	{
		WriteLine( ID_INFO, message, tag );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  message  The message
	 * @param[in]  tag      The tag
	 */
	void Debug( string message, string tag = "" )
	{
		WriteLine( ID_DEBUG, message, tag );		
	}
}