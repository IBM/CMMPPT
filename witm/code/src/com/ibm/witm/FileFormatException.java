
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File FileFormatException.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

/*******************************************************************************
 * Indicates that an error occurred when attempting to read the file for an
 * InputTable. The detail message is designed to be usable as an error message
 * displayed to the end-user. 
 * @see InputTable
 *******************************************************************************
 */
public final class FileFormatException extends RuntimeException
{
////////////////////////////////////////////////////////////////////////////////
// Private Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * This field is required in order to avoid a compiler warning.<br>
 * It is not used by WIT-M.
 *******************************************************************************
 */
   private static final long serialVersionUID = 1L;

////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructors
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @param message The detail message: This should be a meaningful description of
 *                the error suitable for being displayed to the end user.
 *******************************************************************************
 */
   FileFormatException (String message)
      {
      super (message);
      }

////////////////////////////////////////////////////////////////////////////////
}
