import React, {useState} from 'react';
import {
  StyleSheet,
  Text,
  Button,
  View,
  Image,
  ScrollView,
  Dimensions,
  Alert,
  Modal,
  Pressable,
  TouchableHighlight,
  Platform,
  TouchableOpacitiy,
  TextInput,
} from 'react-native';
export const PageContext = React.createContext();

export const PageContextProvider = () => {
  const [connected, setConnected] = useState(false);
  const [name, setName] = useState('katz');

  return (
    <PageContext.Provider value={{connected, setConnected, name, setName}} >
      <Text>
        test
      </Text>
    </PageContext.Provider>
  );
}
