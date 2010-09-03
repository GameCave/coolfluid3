#ifndef CF_GUI_Client_FilesListItem_h
#define CF_GUI_Client_FilesListItem_h

////////////////////////////////////////////////////////////////////////////////

#include <QStandardItem>

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace Client {

////////////////////////////////////////////////////////////////////////////////

  enum FilesListItemType
  {
    /// @brief Directory type.
    DIRECTORY,
    /// @brief File type.
    FILE
  };

////////////////////////////////////////////////////////////////////////////////

  /// @brief Adds a functionnality to @c QStandardItem class.

  /// This class inherits from @c QStandardItem and add only one
  /// functionnality to its base class : the type of this item. An item can
  /// be either a file or a directory and it can be usefull to remember this,
  /// for exemple, to easily manage icons.@n @n
  /// This class is used by @c NRemoteBrowser to create items for
  /// the list view.@n @n
  /// @author Quentin Gasper.

  class FilesListItem : public QStandardItem
  {

    public:

    /// @brief Constructor.

    /// Calls the base class constructor with provided icon and text:
    /// @c QStandardItem(icon, @c text) and sets the provided type
    /// value to @c #type.
    /// @param icon Item icon.
    /// @param text Item text.
    /// @param type Item type.
    /// @throw UnknownType If the type is unknown.
    FilesListItem(const QIcon & icon, const QString & text, FilesListItemType type);

    /// @brief Gives the type of this item.

    /// @return Returns @c DIRECTORY if this item is a directory, otherwise
    /// returns @c FILE.
    FilesListItemType getType() const;

    private:

    /// @brief Indicates the type of this item.

    /// The value is either @c DIRECTORY or @c FILE.
    FilesListItemType m_type;

  }; // class FilesListItemType

////////////////////////////////////////////////////////////////////////////////

} // namespace Client
} // namespace GUI
} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_FilesListItem_h
